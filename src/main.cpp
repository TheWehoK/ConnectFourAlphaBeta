#include "Board.h"
#include "AI.h"
#include <iostream>
#include <limits> 

int main() {
    Board board;
    Cell human = Cell::Red;
    Cell ai = Cell::Yellow;
    AI aiPlayer(ai, human, 5); // depth 5 — adjust later

    std::cout << "You are R. Enter a column (0-6) to drop your piece.\n\n";
    board.print();

    while (true) {
        int col;
    std::cout << "Your move: ";
    if (!(std::cin >> col)) {
        if (std::cin.eof()) {            // input stream closed, so quit instead of looping
            std::cout << "\nNo input available, exiting.\n";
            break;
        }
        std::cin.clear();                // reset the failed state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // throw away the bad text
        std::cout << "Please enter a number from 0 to 6.\n";
        continue;
}

if (col < 0 || col > 6 || !board.dropPiece(col, human)) {
    std::cout << "Invalid move, try again.\n";
    continue;
}

        board.print();
        if (board.checkWin(human)) { std::cout << "You win!\n"; break; }
        if (board.isBoardFull()) { std::cout << "Draw!\n"; break; }

        int aiCol = aiPlayer.getBestMoveAlphaBeta(board);
        board.dropPiece(aiCol, ai);
        std::cout << "AI plays column " << aiCol
                   << " (explored " << aiPlayer.getNodesExplored() << " nodes)\n";
        board.print();

        if (board.checkWin(ai)) { std::cout << "AI wins!\n"; break; }
        if (board.isBoardFull()) { std::cout << "Draw!\n"; break; }
    }

    return 0;
}