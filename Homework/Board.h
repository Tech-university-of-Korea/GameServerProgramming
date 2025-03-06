#pragma once

class Board {
public:
    inline static constexpr size_t BOARD_CELLS = 8;

public:
    Board(INT32 windowWidth, INT32 windowHeight);
    ~Board();

public:
    // 윈도우 크기 변경
    void OnResize(INT32 windowWidth, INT32 windowHeight);

    // 업데이트 (체스 말 이동)
    void Update();

    // 체스판 그리기
    void Render(Gdiplus::Graphics* backbuffer);

private:
    Gdiplus::SizeF mCellSize;
    std::array<std::array<Gdiplus::PointF, 8>, 8> mBoard;

    Gdiplus::Point mChessPiecePosition{ 0, 0 };
    Gdiplus::Bitmap mChessPiece;
};