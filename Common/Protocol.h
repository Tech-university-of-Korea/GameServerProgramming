#pragma once

// 03 26
#include "Types.h"

inline const char* LOOP_BACK_IPv4 = "127.0.0.1";

inline constexpr uint8_t BOARD_SIZE = 8;
inline constexpr uint16_t SERVER_PORT = 7777;

enum PacketType : uint8_t {
    PACKET_TYPE_INPUT,
    PACKET_TYPE_NOTIFY_ID,
    PACKET_TYPE_PLAYER_POSITION,
    PACKET_TYPE_PLAYER_ENTER,
    PACKET_TYPE_PLAYER_EXIT
};

struct PacketHeader {
    PacketSizeType size;
    PacketType type;
    SessionIdType senderId;
};

struct PacketKeyInput : public PacketHeader {
    uint8_t key; // key 
};

struct PacketNotifyId : public PacketHeader { };

struct PacketPlayerPos : public PacketHeader {
    Byte2 pos;
};

struct PacketPlayerEnter : public PacketHeader {
    Byte2 pos;
};

struct PacketPlayerExit : public PacketHeader { };