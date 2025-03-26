#pragma once

// 03 26
#include <stdint.h> // uintN_t, intN_t 등의 자료형때문에 추가

inline const char* LOOP_BACK_IPv4 = "127.0.0.1";

inline constexpr uint8_t BOARD_SIZE = 8;
inline constexpr uint16_t SERVER_PORT = 7777;

// 숙제에 필요한 정보는 키입력 및 플레이어의 위치정보 밖에 없다.
// 따로 구조화할 필요는 X
// 서버 -> 무조건 KeyInput만 Recv, 클라 -> 무조건 위치만 Recv
struct PacketKeyInput {
    uint8_t key; // key
};

struct PacketPlayerPos {
    uint8_t x;
    uint8_t y;
};