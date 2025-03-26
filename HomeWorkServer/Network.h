#pragma once

class Network {
public:
    Network();
    ~Network();

public:
    void Run();
    void ProcessKeyInput(uint8_t key);

private:
    SOCKET mListenSocket{ INVALID_SOCKET };
    SOCKET mClientSocket{ INVALID_SOCKET };

    int8_t mPlayerX{ 0 };
    int8_t mPlayerY{ 0 };

    sockaddr_in mServerAddr{ };
};
