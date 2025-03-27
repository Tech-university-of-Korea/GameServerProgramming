#pragma once

#include "targetver.h"

#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// Windows ��� ����
#include <windows.h>
#include <CommCtrl.h>

// C ��Ÿ�� ��� �����Դϴ�.
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