# Programming Assignment 1: Connect Four with Alpha-Beta Pruning
 
**Track:** B — Implementation and Building Something
**Aarav Khemani**
 
---
 
## (a) What I Built
 
I implemented minimax search with alpha-beta pruning for Connect Four, built into a command-line game that can be played against the AI.
 
**Language:** C++17, built with CMake.
 
**Representation:** The board is a 2D vector, `std::vector<std::vector<Cell>>`, indexed `[column][row]`, where each `Cell` is an enum (`Empty`, `Red`, `Yellow`). Storing it column-first makes dropping a piece simple: scan upward from row 0 in the chosen column and place the piece in the first empty slot.
 
**Core components:**
- `Board` — grid state, legal move generation, and win detection in all four directions (horizontal, vertical, and both diagonals).
- `AI` — two search functions, `minimax` and `alphaBeta`, sharing the same structure so their behaviour can be compared directly. Both alternate between maximising (the AI's turn) and minimising (the opponent's turn) down to a configurable search depth.
- `evaluate()` — scores positions by scanning every possible four-in-a-row "window" on the board and scoring it by how many of each player's pieces occupy it, plus a centre-column control bonus, since centre pieces sit in a stronger position than those on the edges.
**Key design decisions and simplifications:**
- Each recursive call copies the board rather than making and undoing a move in place. This is simpler to get correct, at the cost of speed.
- I exposed search depth, who moves first, and which algorithm to use as command-line flags (`--depth`, `--first`, `--algo`) rather than hardcoding them, since depth in particular is a focus of the algorithm and affects both playing strength and response time, and a user should be able to trade one against the other.
---
 
## (b) The Tool
 
The tool is a playable terminal game (`connect_four`). A human plays as Red, the AI plays as Yellow (or vice versa, depending on `--first`), and the board is printed after every move.
 
**Interface decisions:**
- `--depth N` controls how many moves ahead the AI searches. Depth 0 was originally accepted and caused the search to run away unboundedly (see the AI use section below); it is now rejected with a warning and clamped to a minimum of 1.
- `--first human|ai` decides who moves first, since testing the AI's opening move is different from testing its response to a human opening.
- `--algo alphabeta|minimax` lets the same game be run with either search function, which is what makes the node-count comparison in the benchmark possible without maintaining two separate programs.
- After each AI move, the tool prints how many positions were explored to reach that decision, so the effect of pruning is visible during normal play, not just in the benchmark.
**Worked example:**
 
ADD HERE
 
---
 
## (c) What I Learned
 
- I was surprised that the AI was hard to beat even at a moderate search depth. I had expected a simple evaluation function (piece windows plus a centre bonus) to play noticeably weaker than it did. I had to lower the depth to 1 before I was able to beat it.
- I learned that alpha-beta pruning is a lot simpler than I expected. I was under the misconception that it would be a completely different way of deciding the best move, and later realised it is only pruning, as the name suggests, the minimax algorithm rather than replacing it.
---
 
## (d) AI Use
 
I used Claude for most of the implementation: the `Board` and `AI` classes, the CMake setup, the test suite, and debugging build errors as they came up. I used it conversationally throughout, checking and rebuilding after each change rather than accepting large blocks of code untested. I also used Claude to edit and rewrite this report and update formatting throughout the README before submitting.
 
**Specific examples of the AI being wrong or incomplete:**
 
1. **Missing header declaration.** When Claude added the `scoreWindow` helper function to `AI.cpp` for the evaluation function, it only mentioned in passing that the declaration also needed adding to `AI.h`, rather than giving the full updated header. I missed this, tried to build, and got a "not defined" error. Separately, when I added the declaration myself, I wrote `ScoreWindow` (capital S) while the `.cpp` file used `scoreWindow` (lowercase); these were treated as two different functions, and I got the same error again until I matched the casing exactly.
2. **The depth-0 bug.** Claude's original CLI code accepted `--depth 0` without validation. The search functions stop when `depth == 0`, but the first recursive call is always made with `maxDepth - 1`, so a depth of 0 immediately became `-1` and then kept decreasing, never hitting exactly 0 again. The only thing left to stop the recursion was the game itself ending, meaning depth 0 turned into "search every possible sequence of moves to the end of the game", and the program just hung with no error message. This wasn't caught by the test suite Claude wrote either, since none of the tests tried an invalid depth. I fixed it by clamping depth to a minimum of 1 in the argument parser, and separately changed the search functions' base case from `depth == 0` to `depth <= 0` so the engine itself cannot run away even if called incorrectly again in the future.
**What I understood versus what I took on trust:**
- I understand the board representation, the win logic, and minimax/alpha-beta. I can confidently explain why the `alpha >= beta` check is the entire pruning mechanism and why it never changes the chosen move.
- I don't fully understand why Claude used the specific numbers it did for the evaluation function, for example the 50 and 60. I understand these numbers need to be higher to weigh more heavily, but I don't know why it chose those specific amounts. I also don't fully understand CMake, having never used it before this project. I know it generates files of some sort to help with building, but not the details of how.
- I'm also trusting that Claude knows how to use proper Australian English as I have formatted and fixed grammer and Typos with it throughout the report.