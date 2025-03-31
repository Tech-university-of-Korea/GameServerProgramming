#pragma once

#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include <Windows.h>

#undef max
#undef min

#include <iostream>
#include <format>

#include <algorithm>

#include <unordered_map>

#include "../Common/Utils.h"
#include "../Common/Protocol.h"

extern std::unique_ptr<class Network> gNetwork;