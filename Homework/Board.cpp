#include "pch.h"
#include "Board.h"
#include "Input.h"

Board::Board(INT32 windowWidth, INT32 windowHeight)
    : mCellSize{ windowWidth / static_cast<float>(BOARD_CELLS), windowHeight / static_cast<float>(BOARD_CELLS) }, mChessPiece{ L"w-knight.png" } {
    OnResize(windowWidth, windowHeight);
}

Board::~Board() { }

void Board::OnResize(INT32 windowWidth, INT32 windowHeight) {
    mCellSize = { windowWidth / static_cast<float>(BOARD_CELLS), windowHeight / static_cast<float>(BOARD_CELLS) };
    for (int row = 0; row < BOARD_CELLS; ++row) {
        for (int col = 0; col < BOARD_CELLS; ++col) {
            auto& [cellX, cellY] = mBoard[row][col];

            cellX = mCellSize.Width * static_cast<float>(col);
            cellY = mCellSize.Height * static_cast<float>(row);
        }
    }
}

void Board::Update() {
    if (Input::IsDown(VK_LEFT)) {
        mChessPiecePosition.X -= 1;
    }

    if (Input::IsDown(VK_RIGHT)) {
        mChessPiecePosition.X += 1;
    }

    if (Input::IsDown(VK_DOWN)) {
        mChessPiecePosition.Y += 1;
    }

    if (Input::IsDown(VK_UP)) {
        mChessPiecePosition.Y -= 1;
    }

    mChessPiecePosition.X = std::clamp(mChessPiecePosition.X, 0, static_cast<int>(BOARD_CELLS - 1));
    mChessPiecePosition.Y = std::clamp(mChessPiecePosition.Y, 0, static_cast<int>(BOARD_CELLS - 1));
}

void Board::Render(Gdiplus::Graphics* backbuffer) {
    Gdiplus::SolidBrush lightBrown{ Gdiplus::Color{ 0xFF, 0xD2, 0xA5, 0x7C } };
    Gdiplus::SolidBrush brown{ Gdiplus::Color{ 0xFF, 0xB0, 0x73, 0x46 } };
    for (int row = 0; row < BOARD_CELLS; ++row) {
        for (int col = 0; col < BOARD_CELLS; ++col) {
            auto& [cellX, cellY] = mBoard[row][col];
            if ((row & 1) == (col & 1)) {
                backbuffer->FillRectangle(&lightBrown, cellX, cellY, mCellSize.Width, mCellSize.Height);
            }
            else {
                backbuffer->FillRectangle(&brown, cellX, cellY, mCellSize.Width, mCellSize.Height);
            }
        }
    }

    Gdiplus::PointF cellPoint{ mChessPiecePosition.X * mCellSize.Width, mChessPiecePosition.Y * mCellSize.Height };
    Gdiplus::RectF cellRect{ cellPoint, mCellSize };
    backbuffer->DrawImage(&mChessPiece, cellRect);
}
