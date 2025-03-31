#include "pch.h"
#include "Session.h"
#include "Network.h"

Session::Session() { }

Session::Session(SessionIdType id, SOCKET socket) 
    : mId{ id }, mSocket{ socket } { }

Session::~Session() { 
    ::closesocket(mSocket);
}

SessionIdType Session::GetId() const {
    return mId;
}

Byte2 Session::GetPosition() const {
    return mPos;
}

char* Session::GetBuffer() {
    return mRecvBuffer;
}

void Session::DoSend(SessionIdType senderId, PacketHeader* packet) {
    OverlappedEx* overlapped = new OverlappedEx{ senderId, packet };
    ::WSASend(mSocket, &overlapped->mWsaBuf, 1, 0, 0, &overlapped->mWsaOverlapped, SendCallback);
}

void Session::ProcessKeyInput(uint8_t key) {
    switch (key) {
    case VK_LEFT:
        mPos.x -= 1;
        break;

    case VK_RIGHT:
        mPos.x += 1;
        break;

    case VK_UP:
        mPos.y -= 1;
        break;

    case VK_DOWN:
        mPos.y += 1;
        break;

    default:
        return;
    }

    mPos.x = std::clamp(mPos.x, static_cast<int8_t>(0), static_cast<int8_t>(BOARD_SIZE - 1));
    mPos.y = std::clamp(mPos.y, static_cast<int8_t>(0), static_cast<int8_t>(BOARD_SIZE - 1));
    std::cout << std::format("Player [{}] Position: ({}, {})\n", mId, mPos.x, mPos.y);
}

void Session::DoRecv() {
    DWORD recvFlag = 0;
    mRecvWsabuf[0].buf = mRecvBuffer;
    mRecvWsabuf[0].len = BUF_SIZE;

    std::memset(&mRecvOverlapped, 0, sizeof(WSAOVERLAPPED));
    mRecvOverlapped.hEvent = reinterpret_cast<HANDLE>(mId);
    auto ret = ::WSARecv(mSocket, mRecvWsabuf, 1, 0, &recvFlag, &mRecvOverlapped, RecvCallback);
    if (SOCKET_ERROR == ret) {
        if (WSA_IO_PENDING != ::WSAGetLastError()) {
            PrintErrorMessage();
            exit(-1);
        }
    }
}

