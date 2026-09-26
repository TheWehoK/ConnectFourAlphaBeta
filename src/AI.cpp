#include "AI.h"
#include <climits>
#include <algorithm>

AI::AI(Cell aiPlayer, Cell opponent, int depth)
    : aiPlayer(aiPlayer), opponent(opponent), maxDepth(depth), nodesExplored(0) {}

long AI::getNodesExplored() const { return nodesExplored; }

int AI::evaluate(const Board& board) const {
    if (board.checkWin(aiPlayer)) return 100;
    if (board.checkWin(opponent)) return -100;
    return 0; // placeholder — improve next
}

int AI::minimax(Board board, int depth, bool maximizing) {
    nodesExplored++;
    if (depth == 0 || board.checkWin(aiPlayer) || board.checkWin(opponent) || board.isBoardFull())
        return evaluate(board);

    auto moves = board.getLegalMoves();
    if (maximizing) {
        int best = INT_MIN;
        for (int col : moves) {
            Board next = board;
            next.dropPiece(col, aiPlayer);
            best = std::max(best, minimax(next, depth - 1, false));
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int col : moves) {
            Board next = board;
            next.dropPiece(col, opponent);
            best = std::min(best, minimax(next, depth - 1, true));
        }
        return best;
    }
}

int AI::getBestMoveMinimax(Board board) {
    nodesExplored = 0;
    auto moves = board.getLegalMoves();
    int bestScore = INT_MIN, bestMove = moves[0];
    for (int col : moves) {
        Board next = board;
        next.dropPiece(col, aiPlayer);
        int score = minimax(next, maxDepth - 1, false);
        if (score > bestScore) { bestScore = score; bestMove = col; }
    }
    return bestMove;
}

int AI::alphaBeta(Board board, int depth, int alpha, int beta, bool maximizing) {
    nodesExplored++;
    if (depth == 0 || board.checkWin(aiPlayer) || board.checkWin(opponent) || board.isBoardFull())
        return evaluate(board);

    auto moves = board.getLegalMoves();
    if (maximizing) {
        int best = INT_MIN;
        for (int col : moves) {
            Board next = board;
            next.dropPiece(col, aiPlayer);
            best = std::max(best, alphaBeta(next, depth - 1, alpha, beta, false));
            alpha = std::max(alpha, best);
            if (alpha >= beta) break;
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int col : moves) {
            Board next = board;
            next.dropPiece(col, opponent);
            best = std::min(best, alphaBeta(next, depth - 1, alpha, beta, true));
            beta = std::min(beta, best);
            if (alpha >= beta) break;
        }
        return best;
    }
}

int AI::getBestMoveAlphaBeta(Board board) {
    nodesExplored = 0;
    auto moves = board.getLegalMoves();
    int bestScore = INT_MIN, bestMove = moves[0];
    int alpha = INT_MIN, beta = INT_MAX;
    for (int col : moves) {
        Board next = board;
        next.dropPiece(col, aiPlayer);
        int score = alphaBeta(next, maxDepth - 1, alpha, beta, false);
        if (score > bestScore) { bestScore = score; bestMove = col; }
        alpha = std::max(alpha, bestScore);
    }
    return bestMove;
}