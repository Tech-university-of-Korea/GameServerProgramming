#pragma once

inline constexpr size_t BUF_SIZE = 1024;

struct OverlappedEx {
    WSAOVERLAPPED mWsaOverlapped;
    SessionIdType mSessionId;
    WSABUF mWsaBuf;
    char mSendMessage[BUF_SIZE];

    OverlappedEx(SessionIdType sessionId, PacketHeader* packet);
    ~OverlappedEx();
};