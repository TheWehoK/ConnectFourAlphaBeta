#include "AI.h"
#include <climits>
#include <algorithm>

AI::AI(Cell aiPlayer, Cell opponent, int depth)
    : aiPlayer(aiPlayer), opponent(opponent), maxDepth(depth), nodesExplored(0) {}

long AI::getNodesExplored() const { return nodesExplored; }

int AI::scoreWindow(Cell w[4]) const {
    int aiCount = 0, oppCount = 0, emptyCount = 0;
    for (int i = 0; i < 4; i++) {
        if (w[i] == aiPlayer) aiCount++;
        else if (w[i] == opponent) oppCount++;
        else emptyCount++;
    }

    if (aiCount == 4) return 1000;
    if (oppCount == 4) return -1000;

    int score = 0;
    if (aiCount == 3 && emptyCount == 1) score += 50;
    else if (aiCount == 2 && emptyCount == 2) score += 10;

    if (oppCount == 3 && emptyCount == 1) score -= 60; // slightly weight blocking higher
    else if (oppCount == 2 && emptyCount == 2) score -= 10;

    return score;
}

int AI::evaluate(const Board& board) const {
    if (board.checkWin(aiPlayer)) return 100000;
    if (board.checkWin(opponent)) return -100000;

    int total = 0;
    int COLS = Board::COLS, ROWS = Board::ROWS;

    // Center column bonus — center control is genuinely stronger in Connect Four
    int centerCol = COLS / 2;
    for (int row = 0; row < ROWS; row++)
        if (board.getCell(centerCol, row) == aiPlayer) total += 6;
        else if (board.getCell(centerCol, row) == opponent) total -= 6;

    // Horizontal windows
    for (int col = 0; col <= COLS - 4; col++)
        for (int row = 0; row < ROWS; row++) {
            Cell w[4] = { board.getCell(col, row), board.getCell(col+1, row),
                          board.getCell(col+2, row), board.getCell(col+3, row) };
            total += scoreWindow(w);
        }

    // Vertical windows
    for (int col = 0; col < COLS; col++)
        for (int row = 0; row <= ROWS - 4; row++) {
            Cell w[4] = { board.getCell(col, row), board.getCell(col, row+1),
                          board.getCell(col, row+2), board.getCell(col, row+3) };
            total += scoreWindow(w);
        }

    // Diagonal Up right
    for (int col = 0; col <= COLS - 4; col++)
        for (int row = 0; row <= ROWS - 4; row++) {
            Cell w[4] = { board.getCell(col, row), board.getCell(col+1, row+1),
                          board.getCell(col+2, row+2), board.getCell(col+3, row+3) };
            total += scoreWindow(w);
        }

    // Diagonal Down right
    for (int col = 0; col <= COLS - 4; col++)
        for (int row = 3; row < ROWS; row++) {
            Cell w[4] = { board.getCell(col, row), board.getCell(col+1, row-1),
                          board.getCell(col+2, row-2), board.getCell(col+3, row-3) };
            total += scoreWindow(w);
        }

    return total;
}

int AI::minimax(Board board, int depth, bool maximizing) {
    nodesExplored++;
    if (depth <= 0 || board.checkWin(aiPlayer) || board.checkWin(opponent) || board.isBoardFull())
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
    if (depth <= 0 || board.checkWin(aiPlayer) || board.checkWin(opponent) || board.isBoardFull())
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