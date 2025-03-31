#pragma once

#include "Board.h"

class Graphic {
public:
    Graphic(HWND hwnd, HDC hDC, INT32 windowWidth, INT32 windowHeight);
    ~Graphic();

public:
    Board& GetBoard();

    void OnResize();
    void Update();
    void Render();

private:
    HWND mHwnd;
    HDC mHDC;
    std::unique_ptr<Gdiplus::Bitmap> mBackBuffer{ nullptr }; // ����� (������۸�)
    Gdiplus::Color mBGColor;        // ����
    std::unique_ptr<Gdiplus::Graphics> mMainGraphic{ nullptr };
    
    Board mBoard;
};

