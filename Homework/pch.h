#pragma once

#include "targetver.h"

#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// Windows 헤더 파일
#include <windows.h>
#include <CommCtrl.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus")

#include <array>
#include <memory>

#include <algorithm>

#include "../Common/Protocol.h"
#include "../Common/Utils.h"

extern SOCKET gSocket;