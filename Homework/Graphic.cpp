#include "pch.h"
#include "Graphic.h"

Graphic::Graphic(HWND hwnd, HDC hDC, INT32 windowWidth, INT32 windowHeight)
    : mHwnd{ hwnd }, mHDC{ hDC }, mMainGraphic{ hDC }, mBoard{ windowWidth, windowHeight } { 
    mBackBuffer = std::make_unique<Gdiplus::Bitmap>(windowWidth, windowHeight);
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
    mBoard.OnResize(rect.right, rect.bottom);
}

void Graphic::Update() {
    mBoard.Update();
}

void Graphic::Render() {
    RECT rect;
    ::GetClientRect(mHwnd, &rect);

    Gdiplus::Graphics* graphic = Gdiplus::Graphics::FromImage(mBackBuffer.get());
    graphic->Clear(Gdiplus::Color{ 0xFF, 0xFF, 0xFF, 0xFF });
    
    Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0), 1.0f);

    // Ã¼½ºÆÇ
    mBoard.Render(graphic);

    Gdiplus::Rect gdiRect{ rect.left, rect.top,
                            rect.right, rect.bottom };

    mMainGraphic.DrawImage(mBackBuffer.get(), gdiRect);
}
