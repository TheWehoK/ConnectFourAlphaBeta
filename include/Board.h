#pragma once
#include <vector>

enum class Cell { Empty, Red, Yellow };

class Board {
public:
    static const int COLS = 7;
    static const int ROWS = 6;

    Board();

    bool dropPiece(int col, Cell player);
    bool isColumnFull(int col) const;
    bool isBoardFull() const;
    bool checkWin(Cell player) const;
    std::vector<int> getLegalMoves() const;
    void print() const;
    Cell getCell(int col, int row) const;

private:
    std::vector<std::vector<Cell>> grid;
};