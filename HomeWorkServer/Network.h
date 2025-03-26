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

    uint8_t mPlayerX{ 0 };
    uint8_t mPlayerY{ 0 };
};
