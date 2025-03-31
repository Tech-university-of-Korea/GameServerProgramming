#include "pch.h"
#include "ClientNetwork.h"
#include "Graphic.h"
#include "Board.h"

ClientNetwork::ClientNetwork() { }

ClientNetwork::~ClientNetwork() {
    CloseNetwork();
}

bool ClientNetwork::InitNetwork(DWORD ip, uint16_t port) {
    WSADATA wsaData;
    if (0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        return false;
    }

    mSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, NULL, NULL);
    if (INVALID_SOCKET == mSocket) {
        return false;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(port);
    addr.sin_addr.s_addr = ::htonl(ip);

    if (SOCKET_ERROR == ::WSAConnect(mSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in), nullptr, nullptr, nullptr, nullptr)) {
        return false;
    }

    u_long nonBlocking = 1;
    ::ioctlsocket(mSocket, FIONBIO, &nonBlocking);

    return true;
}

void ClientNetwork::CloseNetwork() {
    ::closesocket(mSocket);
    ::WSACleanup();
}

void ClientNetwork::DoSend(PacketHeader* packet) {
    packet->senderId = mId;
    auto ret = ::send(mSocket, reinterpret_cast<char*>(packet), packet->size, 0);
    if (WSAEWOULDBLOCK == ret) {
        auto errorCode = ::WSAGetLastError();
        if (WSAEWOULDBLOCK != errorCode) {
            PopupErrorMessage();
            ::exit(EXIT_FAILURE);
        }

        DoSend(packet);
    }
}

void ClientNetwork::DoRecv() {
    mRecvBytes = ::recv(mSocket, mRecvBuffer + mRemainBytes, BUF_SIZE, 0);
    if (SOCKET_ERROR == mRecvBytes) {
        auto errorCode = ::WSAGetLastError();
        if (WSAEWOULDBLOCK != errorCode) {
            PopupErrorMessage();
            ::exit(EXIT_FAILURE);
        }

        return;
    }

    if (0 == mRecvBytes) {
        ::closesocket(mSocket);
        ::PostQuitMessage(0);
    }

    CheckAndProcessPacket();
}

void ClientNetwork::ProcessPacket(PacketHeader* packet) {
    switch (packet->type) {
    case PACKET_TYPE_NOTIFY_ID:
        {
            mId = packet->senderId;
            graphic->GetBoard().AddChessPiece(mId, { 0, 0 });
        }
        break;

    case PACKET_TYPE_PLAYER_ENTER:
        {
            if (mId != packet->senderId) {
                auto enterPacket = reinterpret_cast<PacketPlayerEnter*>(packet);
                graphic->GetBoard().AddChessPiece(enterPacket->senderId, enterPacket->pos);
            }
        }
        break;

    case PACKET_TYPE_PLAYER_POSITION:
        {
            auto posPacket = reinterpret_cast<PacketPlayerPos*>(packet);
            graphic->GetBoard().MoveChessPiece(posPacket->senderId, posPacket->pos);
        }
        break;

    case PACKET_TYPE_PLAYER_EXIT:
        {
            graphic->GetBoard().EraseChessPiece(packet->senderId);
        }
        break;

    default:
        break;
    }
}

void ClientNetwork::CheckAndProcessPacket() {
    char* iter = mRecvBuffer;

    mRemainBytes = mRecvBytes;
    PacketHeader* packet{ };
    while (iter < mRecvBuffer + mRecvBytes) {
        packet = reinterpret_cast<PacketHeader*>(iter);
        iter = iter + packet->size;
        mRemainBytes -= packet->size;

        ProcessPacket(packet);
    }

    if (mRemainBytes > 0) {
        std::memmove(mRecvBuffer, iter, mRemainBytes);
    }
}

