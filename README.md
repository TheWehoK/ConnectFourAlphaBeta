# Connect Four AI (Minimax + Alpha-Beta Pruning)

A command-line Connect Four game where you play against an AI opponent powered by
minimax search with alpha-beta pruning.

## What this is

The AI looks ahead a configurable number of moves, assuming both players play
optimally, and picks the move that gives it the best guaranteed outcome. Alpha-beta
pruning is used to skip exploring branches that can't affect the final decision,
which produces the same move as plain minimax but with dramatically fewer positions
examined.

## Requirements

- A C++17 compiler (tested with MSVC via Visual Studio 2026)
- CMake 3.10 or later

## Building

```bash
git clone <your-repo-url>
cd ConnectFourAlphaBeta
cmake -S . -B build
cmake --build build
```

This produces two executables under `build/` (or `build/Debug/` on Windows with
Visual Studio):

- `connect_four` — the playable game
- `tests` — a small suite of correctness tests

## Running the game

```bash
./build/Debug/connect_four.exe [options]
```

### Options

| Flag | Values | Default | Description |
|---|---|---|---|
| `--depth N` | any integer greater than 1 | `5` | How many moves ahead the AI searches. Higher = stronger and slower. |
| `--first` | `human` \| `ai` | `human` | Who moves first. |
| `--algo` | `alphabeta` \| `minimax` | `alphabeta` | Which search algorithm the AI uses. |

### Example

```bash
./build/Debug/connect_four.exe --depth 6 --first ai --algo minimax
```

The board is printed after every move, with `R` for the human's pieces and `Y` for
the AI's. Columns are entered by number, 0 (leftmost) to 6 (rightmost). The AI's move
is printed along with how many positions it explored to make its decision.

## Running the tests

```bash
./build/Debug/tests.exe
```

Prints a `PASS` line for each check, covering win detection in all four directions,
full-column and full-board handling, the AI taking an immediate win, the AI blocking
an immediate loss, and minimax/alpha-beta agreeing on the same move.
