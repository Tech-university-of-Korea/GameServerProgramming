#include "pch.h"
#include "Network.h"
#include "OverlappedEx.h"

void CALLBACK RecvCallback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags)
{
    auto sessionid = reinterpret_cast<int64_t>(over->hEvent);

    decltype(auto) session = gNetwork->GetSession(static_cast<SessionIdType>(sessionid));

    if (num_bytes <= 0) {
        gNetwork->EraseSession(sessionid);
    }
    else {
        char* recvBuffer = session.GetBuffer();
        gNetwork->CheckAndProcessPacket(recvBuffer, num_bytes);

        session.DoRecv();
    }
}

void CALLBACK SendCallback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags)
{
    OverlappedEx* overlapped = reinterpret_cast<OverlappedEx*>(over);
    delete overlapped;
}

Network::Network() {
    WSADATA wsaData;
    if (0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        std::cout << "WSAStartup failed. Unable to initialize Winsock." << std::endl;
        ::exit(EXIT_FAILURE);
    }

    mListenSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, NULL, WSA_FLAG_OVERLAPPED);
    if (INVALID_SOCKET == mListenSocket) {
        std::cout << "Failed to create socket." << std::endl;
        ::exit(EXIT_FAILURE);
    }

    DWORD reuse = 1;
    if (SOCKET_ERROR == ::setsockopt(mListenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse))) {
        PrintErrorMessage();
        ::exit(EXIT_FAILURE);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(SERVER_PORT);
    addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    if (SOCKET_ERROR == ::bind(mListenSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in))) {
        std::cout << "Bind failed. Unable to assign address to socket." << std::endl;
        PrintErrorMessage();
        ::exit(EXIT_FAILURE);
    }

    if (SOCKET_ERROR == ::listen(mListenSocket, SOMAXCONN)) {
        std::cout << "Listen failed. Unable to start listening on socket." << std::endl;
        ::exit(EXIT_FAILURE);
    }
}

Network::~Network() {
    ::closesocket(mListenSocket);
    ::WSACleanup();
}

Session& Network::GetSession(SessionIdType id) {
    return mSessionMap[id];
}

SessionIdType Network::GetSessionCount() const {
    return static_cast<SessionIdType>(mSessionMap.size());
}

void Network::Run() {
    int64_t clientId{ 0 };
    for (; ; ++clientId) {
        SOCKET socket = ::WSAAccept(mListenSocket, nullptr, 0, nullptr, NULL);
        if (INVALID_SOCKET != socket) {
            std::cout << "Accept!" << std::endl;
            if (10 <= gNetwork->GetSessionCount()) {
                std::cout << "Max Session Connected!!" << std::endl;
                continue;
            }
            gNetwork->AddSession(clientId, socket);
        }
    }
}

void Network::CheckAndProcessPacket(char* recvBuffer, DWORD numBytes) {
    char* iter = recvBuffer;
    PacketHeader* packet;
    while (iter < recvBuffer + numBytes) {
        packet = reinterpret_cast<PacketHeader*>(iter);
        iter = iter + packet->size;

        if (not mSessionMap.contains(packet->senderId)) {
            continue;
        }

        ProcessPacket(packet);
    }
}

void Network::ProcessPacket(PacketHeader* packet) {
    switch (packet->type) {
    case PacketType::PACKET_TYPE_INPUT:
        {
            decltype(auto) session = mSessionMap[packet->senderId];
            session.ProcessKeyInput(reinterpret_cast<PacketKeyInput*>(packet)->key);
            PacketPlayerPos posPacket{ sizeof(PacketPlayerPos), PACKET_TYPE_PLAYER_POSITION, session.GetId(), session.GetPosition() };
            
            BroadCast(&posPacket);
        }
        break;

    default:
        break;
    }
}


void Network::AddSession(SessionIdType id, SOCKET socket) {
    mSessionMap.try_emplace(id, id, socket);
    decltype(auto) newSession = mSessionMap[id];

    std::cout << std::format("Session Connect! Id: {}\n", id);

    PacketNotifyId idPacket{ sizeof(PacketNotifyId), PACKET_TYPE_NOTIFY_ID, id };
    newSession.DoSend(id, &idPacket);

    PacketPlayerEnter enterPacket{ sizeof(PacketPlayerEnter), PACKET_TYPE_PLAYER_ENTER };
    for (auto& [id, session] : mSessionMap) {
        if (id == newSession.GetId()) {
            continue;
        }

        enterPacket.senderId = id;
        enterPacket.pos = session.GetPosition();

        newSession.DoSend(id, &enterPacket);
    }

    PacketPlayerEnter newSessionEnterPacket{ sizeof(PacketPlayerEnter), PACKET_TYPE_PLAYER_ENTER };
    newSessionEnterPacket.senderId = newSession.GetId();
    newSessionEnterPacket.pos = newSession.GetPosition();
    BroadCast(&newSessionEnterPacket);

    newSession.DoRecv();
}

void Network::EraseSession(SessionIdType id) {
    if (mSessionMap.contains(id)) {
        mSessionMap.erase(id);
    }

    std::cout << std::format("Session Disconnect! Id: {}\n", id);

    PacketPlayerExit exitPacket{ sizeof(PacketPlayerExit), PACKET_TYPE_PLAYER_EXIT, id };
    BroadCast(&exitPacket);
}

void Network::BroadCast(PacketHeader* packet) {
    for (auto& [id, session] : mSessionMap) {
        session.DoSend(id, packet);
    }
}
