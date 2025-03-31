#pragma once

class ChessPiece {
public:
    ChessPiece();
    ChessPiece(const std::wstring& imageFilePath);
    ChessPiece(const std::wstring& imageFilePath, Byte2 pos);
    ~ChessPiece();

public:
    void SetPosition(Byte2 pos);
    void SetPosition(int8_t x, int8_t y);
    Byte2 GetPosition() const;

    void Render(const Gdiplus::RectF& drawRect, Gdiplus::Graphics* backBuffer);

private:
    Gdiplus::Bitmap mImage;
    Byte2 mPosition{ 0, 0 };
};

