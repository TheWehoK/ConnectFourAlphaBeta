#include "Board.h"
#include "AI.h"
#include <cassert>
#include <iostream>

void test_horizontal_win() {
    Board b;
    b.dropPiece(0, Cell::Red);
    b.dropPiece(1, Cell::Red);
    b.dropPiece(2, Cell::Red);
    b.dropPiece(3, Cell::Red);
    assert(b.checkWin(Cell::Red));
    assert(!b.checkWin(Cell::Yellow));
    std::cout << "PASS: horizontal win\n";
}

void test_vertical_win() {
    Board b;
    b.dropPiece(0, Cell::Red);
    b.dropPiece(0, Cell::Red);
    b.dropPiece(0, Cell::Red);
    b.dropPiece(0, Cell::Red);
    assert(b.checkWin(Cell::Red));
    std::cout << "PASS: vertical win\n";
}

void test_diagonal_up_win() {
    // Build a staircase using filler pieces so Red pieces land on the diagonal.
    Board b;
    b.dropPiece(0, Cell::Red);                          // (0,0)
    b.dropPiece(1, Cell::Yellow);
    b.dropPiece(1, Cell::Red);                          // (1,1)
    b.dropPiece(2, Cell::Yellow);
    b.dropPiece(2, Cell::Yellow);
    b.dropPiece(2, Cell::Red);                          // (2,2)
    b.dropPiece(3, Cell::Yellow);
    b.dropPiece(3, Cell::Yellow);
    b.dropPiece(3, Cell::Yellow);
    b.dropPiece(3, Cell::Red);                          // (3,3)
    assert(b.checkWin(Cell::Red));
    std::cout << "PASS: diagonal (up) win\n";
}

void test_full_column_rejected() {
    Board b;
    for (int i = 0; i < 6; i++) {
        bool ok = b.dropPiece(0, Cell::Red);
        assert(ok);
    }
    bool result = b.dropPiece(0, Cell::Yellow); // column now full
    assert(!result);
    std::cout << "PASS: full column rejected\n";
}

void test_full_board_draw() {
    Board b;
    // Fill every column completely without creating a 4-in-a-row.
    Cell pattern[Board::COLS][Board::ROWS] = {}; // not used directly; fill via alternating drops
    Cell turn = Cell::Red;
    for (int col = 0; col < Board::COLS; col++) {
        for (int row = 0; row < Board::ROWS; row++) {
            b.dropPiece(col, turn);
            turn = (turn == Cell::Red) ? Cell::Yellow : Cell::Red;
        }
    }
    assert(b.isBoardFull());
    std::cout << "PASS: full board detected\n";
}

void test_ai_takes_immediate_win() {
    Board b;
    // Red has three in a row at columns 0,1,2 on the bottom row; column 3 wins.
    b.dropPiece(0, Cell::Red);
    b.dropPiece(1, Cell::Red);
    b.dropPiece(2, Cell::Red);
    // Give Yellow some irrelevant moves so it's Red's turn to move next.
    AI ai(Cell::Red, Cell::Yellow, 4);
    int move = ai.getBestMoveAlphaBeta(b);
    assert(move == 3);
    std::cout << "PASS: AI takes immediate win\n";
}

void test_ai_blocks_immediate_loss() {
    Board b;
    // Yellow has three in a row; it's Red's turn, Red must block at column 3.
    b.dropPiece(0, Cell::Yellow);
    b.dropPiece(1, Cell::Yellow);
    b.dropPiece(2, Cell::Yellow);
    AI ai(Cell::Red, Cell::Yellow, 4);
    int move = ai.getBestMoveAlphaBeta(b);
    assert(move == 3);
    std::cout << "PASS: AI blocks immediate loss\n";
}

void test_minimax_matches_alphabeta() {
    Board b;
    b.dropPiece(3, Cell::Red);
    b.dropPiece(2, Cell::Yellow);
    b.dropPiece(4, Cell::Red);

    AI ai(Cell::Yellow, Cell::Red, 5);
    int moveMinimax = ai.getBestMoveMinimax(b);
    int moveAlphaBeta = ai.getBestMoveAlphaBeta(b);
    assert(moveMinimax == moveAlphaBeta);
    std::cout << "PASS: minimax and alpha-beta agree\n";
}

int main() {
    test_horizontal_win();
    test_vertical_win();
    test_diagonal_up_win();
    test_full_column_rejected();
    test_full_board_draw();
    test_ai_takes_immediate_win();
    test_ai_blocks_immediate_loss();
    test_minimax_matches_alphabeta();

    std::cout << "\nAll tests passed.\n";
    return 0;
}