#include "pch.h"
#include "Network.h"

Network::Network() {
    WSADATA wsaData;
    if (0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        std::cout << "WSAStartup failed. Unable to initialize Winsock." << std::endl;
        ::exit(EXIT_FAILURE);
    }

    mListenSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, NULL, NULL);
    if (INVALID_SOCKET == mListenSocket) {
        std::cout << "Failed to create socket." << std::endl;
        ::exit(EXIT_FAILURE);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(SERVER_PORT);
    addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    if (SOCKET_ERROR == ::bind(mListenSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in))) {
        std::cout << "Bind failed. Unable to assign address to socket." << std::endl;
        ::exit(EXIT_FAILURE);
    }

    if (SOCKET_ERROR == ::listen(mListenSocket, SOMAXCONN)) {
        std::cout << "Listen failed. Unable to start listening on socket." << std::endl;
        ::exit(EXIT_FAILURE);
    }
}

Network::~Network() {
    ::closesocket(mClientSocket);
    ::closesocket(mListenSocket);
    ::WSACleanup();
}

void Network::Run() {
    sockaddr_in remoteAddr{ };
    int32_t addrLen{ sizeof(sockaddr_in) };

    mClientSocket = ::WSAAccept(mListenSocket, reinterpret_cast<sockaddr*>(&remoteAddr), &addrLen, nullptr, NULL);
    if (INVALID_SOCKET == mClientSocket) {
        std::cout << "Accept Failed." << std::endl;
        PrintErrorMessage();
        ::exit(EXIT_FAILURE);
    }

    char clientIP[INET_ADDRSTRLEN]{ };
    ::inet_ntop(AF_INET, &remoteAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
    std::cout << std::format("Client [{}] Connected\n", clientIP);

    PacketKeyInput recvInput{ };
    PacketPlayerPos sendPos{ };
   
    DWORD recvdBytes{ };
    DWORD recvFlag{ 0 };

    WSABUF recvWsaBuf{ };
    WSABUF sendWsaBuf{ };
    recvWsaBuf.buf = reinterpret_cast<char*>(&recvInput);
    recvWsaBuf.len = sizeof(PacketKeyInput);

    sendWsaBuf.buf = reinterpret_cast<char*>(&sendPos);
    sendWsaBuf.len = sizeof(PacketPlayerPos);

    while (true) {
        std::memset(&recvInput, 0, sizeof(PacketKeyInput));
        auto ret = ::WSARecv(mClientSocket, &recvWsaBuf, 1, &recvdBytes, &recvFlag, nullptr, nullptr);
        if (SOCKET_ERROR == ret) {
            std::cout << "Recv Error" << std::endl;
            PrintErrorMessage();
            break;
        }

        ProcessKeyInput(recvInput.key);

        sendPos.x = mPlayerX;
        sendPos.y = mPlayerY;

        DWORD sentBytes;
        ret = ::WSASend(mClientSocket, &sendWsaBuf, 1, &sentBytes, 0, nullptr, nullptr);
        if (SOCKET_ERROR == ret) {
            std::cout << "Send Error" << std::endl;
            PrintErrorMessage();
            break;
        }
    }
}

void Network::ProcessKeyInput(uint8_t key) {
    switch (key) {
    case VK_LEFT:
        std::cout << "Key Input LEFT" << std::endl;
        mPlayerX -= 1;
        break;

    case VK_RIGHT:
        std::cout << "Key Input RIGHT" << std::endl;
        mPlayerX += 1;
        break;

    case VK_UP:
        std::cout << "Key Input UP" << std::endl;
        mPlayerY -= 1;
        break;

    case VK_DOWN:
        std::cout << "Key Input DOWN" << std::endl;
        mPlayerY += 1;
        break;

    default:
        return;
    }

    mPlayerX = std::clamp(mPlayerX, static_cast<int8_t>(0), static_cast<int8_t>(BOARD_SIZE - 1));
    mPlayerY = std::clamp(mPlayerY, static_cast<int8_t>(0), static_cast<int8_t>(BOARD_SIZE - 1));
    std::cout << std::format("Player Position: ({}, {})\n", mPlayerX, mPlayerY);
}
