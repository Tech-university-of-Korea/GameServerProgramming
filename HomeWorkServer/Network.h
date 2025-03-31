#pragma once

#include "Session.h"

class Network {
public:
    Network();
    ~Network();

public:
    Session& GetSession(SessionIdType id);
    SessionIdType GetSessionCount() const;

    void Run();

    void AddSession(SessionIdType id, SOCKET socket);
    void EraseSession(SessionIdType id);

    void CheckAndProcessPacket(char* recvBuffer, DWORD numBytes);
    void ProcessPacket(PacketHeader* packet);
    void BroadCast(PacketHeader* packet);

private:
    SOCKET mListenSocket{ INVALID_SOCKET };

    sockaddr_in mServerAddr{ };
    std::unordered_map<SessionIdType, Session> mSessionMap{ };
};
