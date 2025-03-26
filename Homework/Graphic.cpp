#include "pch.h"
#include "Graphic.h"

Graphic::Graphic(HWND hwnd, HDC hDC, INT32 windowWidth, INT32 windowHeight)
    : mHwnd{ hwnd }, mHDC{ hDC }, mBoard{ windowWidth, windowHeight } { 
    mBackBuffer = std::make_unique<Gdiplus::Bitmap>(windowWidth, windowHeight);
    mMainGraphic = std::make_unique<Gdiplus::Graphics>(hDC);
}

Graphic::~Graphic() { }

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

void Graphic::Update(int8_t x, int8_t y) {
    mBoard.Update(x, y);
}

void Graphic::Render() {
    RECT rect;
    ::GetClientRect(mHwnd, &rect);

    Gdiplus::Graphics* graphic = Gdiplus::Graphics::FromImage(mBackBuffer.get());
    graphic->Clear(Gdiplus::Color{ 0xFF, 0xFF, 0xFF, 0xFF });
    
    // ü����
    mBoard.Render(graphic);

    Gdiplus::Rect gdiRect{ rect.left, rect.top,
                            rect.right, rect.bottom };

    mMainGraphic->DrawImage(mBackBuffer.get(), gdiRect);
}
