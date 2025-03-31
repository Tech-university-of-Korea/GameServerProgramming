#include "pch.h"
#include "Input.h"
#include "ClientNetwork.h"

bool Input::IsDown(KeyIdxType key) {
    return KeyState::DOWN == mKeyboard[key];
}

bool Input::IsPress(KeyIdxType key) {
    return KeyState::PRESS == mKeyboard[key];
}

bool Input::IsUp(KeyIdxType key) {
    return KeyState::UP == mKeyboard[key];
}

bool Input::IsRelease(KeyIdxType key) {
    return KeyState::RELEASE == mKeyboard[key];
}

void Input::UpdateKeyState(bool down, KeyIdxType key) {
    auto& state = mKeyboard[key];
    if (down) {
        switch (state) {
        case KeyState::DOWN:
            state = KeyState::PRESS;
            break;

        case KeyState::UP:
        case KeyState::RELEASE:
            state = KeyState::DOWN;
            break;

        default:
            break;
        }
    }
    else {
        switch (state) {
        case KeyState::UP:
            state = KeyState::RELEASE;
            break;

        case KeyState::DOWN:
        case KeyState::PRESS:
            state = KeyState::UP;
            break;

        default:
            break;
        }
    }
}

void Input::UpdateKey() {
    static std::array<BYTE, 256> keyboard;

    if (false == ::GetKeyboardState(keyboard.data())) {
        return;
    }

    if (keyboard[VK_ESCAPE] & 0x80) {
        ::PostQuitMessage(0);
    }

    for (KeyIdxType key = 0; key < keyboard.size(); ++key) {
        UpdateKeyState(keyboard[key] & 0x80, key);
    }

    PacketKeyInput inputPacket{ sizeof(PacketKeyInput), PACKET_TYPE_INPUT, };
    if (IsDown(VK_LEFT)) {
        inputPacket.key = VK_LEFT;
        gClientNetwork->DoSend(&inputPacket);
    }
    else if (IsDown(VK_RIGHT)) {
        inputPacket.key = VK_RIGHT;
        gClientNetwork->DoSend(&inputPacket);
    }
    else if (IsDown(VK_UP)) {
        inputPacket.key = VK_UP;
        gClientNetwork->DoSend(&inputPacket);
    }
    else if (IsDown(VK_DOWN)) {
        inputPacket.key = VK_DOWN;
        gClientNetwork->DoSend(&inputPacket);
    }
}
