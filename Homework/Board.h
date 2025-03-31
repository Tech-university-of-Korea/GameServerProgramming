#pragma once

#include "ChessPiece.h"

class Board {
public:
    inline static constexpr size_t BOARD_CELLS = 8;

public:
    Board(INT32 windowWidth, INT32 windowHeight);
    ~Board();

public:
    void AddChessPiece(SessionIdType id, Byte2 pos);
    void AddChessPiece(const std::wstring& imageFile, SessionIdType id, Byte2 pos);
    void EraseChessPiece(SessionIdType id);
    void MoveChessPiece(SessionIdType id, Byte2 pos);

    // ������ ũ�� ����
    void OnResize(INT32 windowWidth, INT32 windowHeight);
    void Update();
    // ü���� �׸���
    void Render(Gdiplus::Graphics* backbuffer);

private:
    Gdiplus::SizeF mCellSize;
    std::array<std::array<Gdiplus::PointF, 8>, 8> mBoard;

    std::unordered_map<SessionIdType, ChessPiece> mChessPieces{ };
};