#pragma once

enum class KeyState : BYTE {
    DOWN,
    PRESS,
    UP,
    RELEASE,
};

// Static
class Input {
public:
    using KeyIdxType = unsigned short; // over 256

public:
    Input() = delete;
    ~Input() = delete;

public:
    static bool IsDown(KeyIdxType key);
    static bool IsPress(KeyIdxType key);
    static bool IsUp(KeyIdxType key);
    static bool IsRelease(KeyIdxType key);

    static void UpdateKeyState(bool down, KeyIdxType key);
    static void UpdateKey();
    
private:
    inline static std::array<KeyState, 256> mKeyboard{ KeyState::RELEASE };
};