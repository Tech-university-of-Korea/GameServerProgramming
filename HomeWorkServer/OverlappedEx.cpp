#include "pch.h"
#include "OverlappedEx.h"

OverlappedEx::OverlappedEx(SessionIdType sessionId, PacketHeader* packet)
    : mSessionId(sessionId) {
    auto packetSize = packet->size;

    ZeroMemory(&mWsaOverlapped, sizeof(mWsaOverlapped));
    mWsaBuf.buf = mSendMessage;
    mWsaBuf.len = packetSize;
    std::memcpy(mSendMessage, packet, packetSize);
}

OverlappedEx::~OverlappedEx() { }
