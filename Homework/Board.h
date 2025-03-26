#pragma once

class Board {
public:
    inline static constexpr size_t BOARD_CELLS = 8;

public:
    Board(INT32 windowWidth, INT32 windowHeight);
    ~Board();

public:
    // ������ ũ�� ����
    void OnResize(INT32 windowWidth, INT32 windowHeight);

    // ������Ʈ (ü�� �� �̵�)
    void Update(int8_t x, int8_t y);

    // ü���� �׸���
    void Render(Gdiplus::Graphics* backbuffer);

private:
    Gdiplus::SizeF mCellSize;
    std::array<std::array<Gdiplus::PointF, 8>, 8> mBoard;

    Gdiplus::Point mChessPiecePosition{ 0, 0 };
    Gdiplus::Bitmap mChessPiece;
};