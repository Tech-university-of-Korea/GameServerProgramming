#pragma once

// 03 26
#include <stdint.h> // uintN_t, intN_t ���� �ڷ��������� �߰�

inline const char* LOOP_BACK_IPv4 = "127.0.0.1";

inline constexpr uint8_t BOARD_SIZE = 8;
inline constexpr uint16_t SERVER_PORT = 7777;

// ������ �ʿ��� ������ Ű�Է� �� �÷��̾��� ��ġ���� �ۿ� ����.
// ���� ����ȭ�� �ʿ�� X
// ���� -> ������ KeyInput�� Recv, Ŭ�� -> ������ ��ġ�� Recv
struct PacketKeyInput {
    uint8_t key; // key
};

struct PacketPlayerPos {
    uint8_t x;
    uint8_t y;
};