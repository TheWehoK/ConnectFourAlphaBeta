#include "Board.h"
#include <iostream>

Board::Board() {
    grid = std::vector<std::vector<Cell>>(COLS, std::vector<Cell>(ROWS, Cell::Empty));
}

bool Board::isColumnFull(int col) const {
    return grid[col][ROWS - 1] != Cell::Empty;
}

bool Board::dropPiece(int col, Cell player) {
    if (isColumnFull(col)) return false;
    for (int row = 0; row < ROWS; row++) {
        if (grid[col][row] == Cell::Empty) {
            grid[col][row] = player;
            return true;
        }
    }
    return false;
}

bool Board::isBoardFull() const {
    for (int col = 0; col < COLS; col++)
        if (!isColumnFull(col)) return false;
    return true;
}

std::vector<int> Board::getLegalMoves() const {
    std::vector<int> moves;
    for (int col = 0; col < COLS; col++)
        if (!isColumnFull(col)) moves.push_back(col);
    return moves;
}

Cell Board::getCell(int col, int row) const {
    return grid[col][row];
}

void Board::print() const {
    for (int row = ROWS - 1; row >= 0; row--) {
        for (int col = 0; col < COLS; col++) {
            char c = '.';
            if (grid[col][row] == Cell::Red) c = 'R';
            else if (grid[col][row] == Cell::Yellow) c = 'Y';
            std::cout << c << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

bool Board::checkWin(Cell player) const {
    // Horizontal
    for (int col = 0; col <= COLS - 4; col++)
        for (int row = 0; row < ROWS; row++)
            if (grid[col][row] == player && grid[col+1][row] == player &&
                grid[col+2][row] == player && grid[col+3][row] == player)
                return true;

    // Vertical
    for (int col = 0; col < COLS; col++)
        for (int row = 0; row <= ROWS - 4; row++)
            if (grid[col][row] == player && grid[col][row+1] == player &&
                grid[col][row+2] == player && grid[col][row+3] == player)
                return true;

    // Diagonal /
    for (int col = 0; col <= COLS - 4; col++)
        for (int row = 0; row <= ROWS - 4; row++)
            if (grid[col][row] == player && grid[col+1][row+1] == player &&
                grid[col+2][row+2] == player && grid[col+3][row+3] == player)
                return true;

    // Diagonal \    
    for (int col = 0; col <= COLS - 4; col++)
        for (int row = 3; row < ROWS; row++)
            if (grid[col][row] == player && grid[col+1][row-1] == player &&
                grid[col+2][row-2] == player && grid[col+3][row-3] == player)
                return true;

    return false;
}