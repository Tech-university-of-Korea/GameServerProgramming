// Homework.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "resource.h"
#include "Input.h"
#include "Graphic.h"
#include "ClientNetwork.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND ipaddrControl;
DWORD ipaddrIP;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:  
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    // GDI+ 초기화 작업입니다.
    ULONG_PTR gdiToken;
    Gdiplus::GdiplusStartupInput gdiStartupInput;
    if (Gdiplus::Ok != Gdiplus::GdiplusStartup(&gdiToken, &gdiStartupInput, NULL)) {
        return FALSE;
    }

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HOMEWORK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (false == InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    RECT windowRect;
    ::GetClientRect(hWnd, &windowRect);
    graphic = new Graphic{ hWnd, ::GetDC(hWnd), windowRect.right, windowRect.bottom };

    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DialogProc);
    gClientNetwork->InitNetwork(ipaddrIP, SERVER_PORT);

    MSG msg;

    while (true) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (WM_QUIT == msg.message) {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            Input::UpdateKey();

            graphic->Update();
            graphic->Render();
        }
    }

    delete graphic;

    // GDI+ 종료작업입니다.
    Gdiplus::GdiplusShutdown(gdiToken);

    gClientNetwork->CloseNetwork();

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOMEWORK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      100, 100, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd) {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_SIZE:
        if (graphic) {
            graphic->OnResize();
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        ipaddrControl = GetDlgItem(hDlg, IDC_IPADDRESS1);
        //ipaddrButton = GetDlgItem(hDlg, IDC_BUTTON1);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            SendMessage(ipaddrControl, IPM_GETADDRESS, 0, (LPARAM)&ipaddrIP);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;

        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;

        default:
            break;
        }
        break;
    }

    return (INT_PTR)FALSE;

}