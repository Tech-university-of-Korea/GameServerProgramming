#include "pch.h"
#include "ChessPiece.h"

ChessPiece::ChessPiece() : mImage{ L"w-knight.png" } { }

ChessPiece::ChessPiece(const std::wstring& imageFilePath)
    : mImage{ imageFilePath.c_str() } { }

ChessPiece::ChessPiece(const std::wstring& imageFilePath, Byte2 pos)
    : mImage{ imageFilePath.c_str() }, mPosition{ pos }  { }

ChessPiece::~ChessPiece() { }

void ChessPiece::SetPosition(Byte2 pos) {
    mPosition = pos;
}

void ChessPiece::SetPosition(int8_t x, int8_t y) {
    mPosition.x = x;
    mPosition.y = y;
}

Byte2 ChessPiece::GetPosition() const {
    return mPosition;
}

void ChessPiece::Render(const Gdiplus::RectF& drawRect, Gdiplus::Graphics* backBuffer) {
    backBuffer->DrawImage(&mImage, drawRect);
}
