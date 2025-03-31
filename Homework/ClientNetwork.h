#pragma once

inline constexpr size_t BUF_SIZE = 1024;

class ClientNetwork {
public:
    ClientNetwork();
    ~ClientNetwork();

public:
    bool InitNetwork(DWORD ip, uint16_t port);
    void CloseNetwork();

    void DoSend(PacketHeader* packet);
    void DoRecv();

private:
    void ProcessPacket(PacketHeader* packet);
    void CheckAndProcessPacket();

public:
    SOCKET mSocket{ INVALID_SOCKET };
    SessionIdType mId{ INVALID_SESSION };

    char mRecvBuffer[BUF_SIZE]{ };
    int32_t mRecvBytes{ };
    int32_t mRemainBytes{ };
};

