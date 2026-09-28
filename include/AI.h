#pragma once
#include "Board.h"

class AI {
public:
    AI(Cell aiPlayer, Cell opponent, int depth);

    int getBestMoveMinimax(Board board);
    int getBestMoveAlphaBeta(Board board);
    long getNodesExplored() const;

private:
    Cell aiPlayer;
    Cell opponent;
    int maxDepth;
    long nodesExplored;

    int scoreWindow(Cell w[4]) const;
    int minimax(Board board, int depth, bool maximizing);
    int alphaBeta(Board board, int depth, int alpha, int beta, bool maximizing);
    int evaluate(const Board& board) const;
};