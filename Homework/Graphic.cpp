#include "pch.h"
#include "Graphic.h"
#include "ClientNetwork.h"

Graphic::Graphic(HWND hwnd, HDC hDC, INT32 windowWidth, INT32 windowHeight)
    : mHwnd{ hwnd }, mHDC{ hDC }, mBoard{ windowWidth, windowHeight } { 
    mBackBuffer = std::make_unique<Gdiplus::Bitmap>(windowWidth, windowHeight);
    mMainGraphic = std::make_unique<Gdiplus::Graphics>(hDC);
}

Graphic::~Graphic() { }

Board& Graphic::GetBoard() {
    return mBoard;
}

void Graphic::OnResize() {
    if (nullptr == mHwnd) {
        return;
    }

    RECT rect;
    ::GetClientRect(mHwnd, &rect);

    mBackBuffer.reset();
    mBackBuffer = std::make_unique<Gdiplus::Bitmap>(rect.right, rect.bottom);

    OutputDebugStringA(std::format("ClientSize is ({}, {})\n", rect.right, rect.bottom).c_str());
    OutputDebugStringA(std::format("mBackBufferSize is ({}, {})\n", mBackBuffer->GetWidth(), mBackBuffer->GetHeight()).c_str());
    mBoard.OnResize(rect.right, rect.bottom);

    mMainGraphic.reset();
    mMainGraphic = std::make_unique<Gdiplus::Graphics>(::GetDC(mHwnd));
}

void Graphic::Update() {
    gClientNetwork->DoRecv();

    mBoard.Update();
}

void Graphic::Render() {
    RECT rect;
    ::GetClientRect(mHwnd, &rect);

    Gdiplus::Graphics* graphic = Gdiplus::Graphics::FromImage(mBackBuffer.get());
    graphic->Clear(Gdiplus::Color{ 0xFF, 0xFF, 0xFF, 0xFF });
    
    // Ã¼½ºÆÇ
    mBoard.Render(graphic);

    Gdiplus::Rect gdiRect{ rect.left, rect.top,
                            rect.right, rect.bottom };

    mMainGraphic->DrawImage(mBackBuffer.get(), gdiRect);
}
