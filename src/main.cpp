#include "Board.h"
#include "AI.h"
#include <iostream>
#include <limits>
#include <string>
#include <cstring>

struct Options {
    int depth = 5;
    bool aiFirst = false;
    bool useMinimax = false; // false = alpha-beta (default)
};

Options parseArgs(int argc, char* argv[]) {
    Options opts;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--depth" && i + 1 < argc) {
            opts.depth = std::stoi(argv[++i]);
        } else if (arg == "--first" && i + 1 < argc) {
            std::string who = argv[++i];
            opts.aiFirst = (who == "ai");
        } else if (arg == "--algo" && i + 1 < argc) {
            std::string algo = argv[++i];
            opts.useMinimax = (algo == "minimax");
        } else if (arg == "--help") {
            std::cout << "Usage: connect_four [--depth N] [--first human|ai] [--algo alphabeta|minimax]\n";
            exit(0);
        }
    }

    if (opts.depth < 1) {
        std::cout << "Depth must be at least 1. Using depth 1.\n";
        opts.depth = 1;
    }

    return opts;
}

int getHumanMove(const Board& board) {
    while (true) {
        int col;
        std::cout << "Your move (0-6): ";
        if (!(std::cin >> col)) {
            if (std::cin.eof()) { std::cout << "\nNo input available, exiting.\n"; exit(0); }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number from 0 to 6.\n";
            continue;
        }
        if (col < 0 || col > 6 || board.isColumnFull(col)) {
            std::cout << "Invalid move, try again.\n";
            continue;
        }
        return col;
    }
}

int main(int argc, char* argv[]) {
    Options opts = parseArgs(argc, argv);

    Cell human = Cell::Red;
    Cell ai = Cell::Yellow;
    AI aiPlayer(ai, human, opts.depth);

    std::cout << "Connect Four - depth " << opts.depth
               << ", algorithm: " << (opts.useMinimax ? "minimax" : "alpha-beta")
               << ", " << (opts.aiFirst ? "AI moves first" : "you move first") << "\n";
    std::cout << "You are R. Enter a column (0-6) to drop your piece.\n\n";

    Board board;
    board.print();

    bool aiTurn = opts.aiFirst;

    while (true) {
        if (aiTurn) {
            int aiCol = opts.useMinimax
                ? aiPlayer.getBestMoveMinimax(board)
                : aiPlayer.getBestMoveAlphaBeta(board);
            board.dropPiece(aiCol, ai);
            std::cout << "AI plays column " << aiCol
                       << " (explored " << aiPlayer.getNodesExplored() << " nodes)\n";
            board.print();

            if (board.checkWin(ai)) { std::cout << "AI wins!\n"; break; }
            if (board.isBoardFull()) { std::cout << "Draw!\n"; break; }
        } else {
            int col = getHumanMove(board);
            board.dropPiece(col, human);
            board.print();

            if (board.checkWin(human)) { std::cout << "You win!\n"; break; }
            if (board.isBoardFull()) { std::cout << "Draw!\n"; break; }
        }
        aiTurn = !aiTurn;
    }

    return 0;
}