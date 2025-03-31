#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Session.h
// 
//  접속중인 클라이언트의 정보를 저장
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "OverlappedEx.h"

extern void CALLBACK SendCallback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags);
extern void CALLBACK RecvCallback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags);

class Session {
public:
    Session();
    Session(SessionIdType id, SOCKET socket);
    ~Session();

public:
    SessionIdType GetId() const;
    Byte2 GetPosition() const;
    char* GetBuffer();

    void ProcessKeyInput(uint8_t key);
    void DoSend(SessionIdType senderId, PacketHeader* packet);
    void DoRecv();

private:
    SessionIdType mId{ };
    SOCKET mSocket{ INVALID_SOCKET };

    Byte2 mPos{ 0, 0 };

    char mRecvBuffer[BUF_SIZE]{ };
    WSABUF mRecvWsabuf[1]{ };
    WSAOVERLAPPED mRecvOverlapped{ };
};