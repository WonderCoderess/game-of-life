# Conway’s Game of Life

A console-based implementation of Conway’s Game of Life in C++. Created as a learning project using dynamic two-dimensional arrays and a game loop.

## Features

- Custom board dimensions from 1 to 50 rows and columns.
- Manual selection of initially live cells.
- Loading the initial board from a text file.
- Step-by-step simulation.
- Detection of dead and stable generations.
- Input validation and manual memory management with `new[]` and `delete[]`.

## Rules

Each cell is either alive or dead. Its neighbors are the eight surrounding positions, including diagonals.

- A live cell survives with 2 or 3 live neighbors.
- Otherwise, it dies.
- A dead cell becomes alive with exactly 3 live neighbors.
- All other dead cells remain dead.

All changes occur simultaneously. Cells outside the board are treated as dead.

The console displays live cells as `#` and dead cells as `.`.

## Build and run

Build `main.cpp` as a C++ console application, for example in Visual Studio, and run it.

The project uses the C++ standard library and requires no external dependencies.

## Input modes

Choose the input mode in `main.cpp`, then rebuild the program:

```cpp
#define USE_FILE_INPUT 0
```

- `0` — manual input.
- `1` — load the board from `board.txt`.

### Manual input

1. Enter the number of rows and columns.
2. Select live cells one at a time by entering a row, then a column.
3. Coordinates start at 1.
4. Enter `0` when asked for a row to finish setup.

All unselected cells remain dead.

### File input

Place `board.txt` in the program’s working directory. When running from Visual Studio, this is normally the project directory.

The first two numbers specify the number of rows and columns. They are followed by one value per cell:

- `0` — dead.
- `1` — alive.

Example:

```text
5 5
0 0 0 0 0
0 0 1 0 0
0 0 1 0 0
0 0 1 0 0
0 0 0 0 0
```

This pattern alternates between a vertical and a horizontal line.

The program validates dimensions and cell values and rejects missing or extra data.

## Included example: Glider

The included `board.txt` contains a glider on a 10 × 10 board. This five-cell pattern moves diagonally, returning to its original shape every four generations, shifted one row down and one column to the right.

Set `USE_FILE_INPUT` to `1` and rebuild the program to load this example. Its behavior changes when it reaches the board’s edge, since cells outside the board are treated as dead.

## Controls

During the simulation:

- Enter `1` to advance by one generation.
- Enter `0` to quit.

After each update, the program stops automatically if all cells are dead or if the new generation is identical to the previous one. These checks occur after the first requested update, including for an initially empty or stable board.

Repeating patterns with periods longer than one, such as the alternating line above, continue until the user quits.

## Program structure

- `Initialize()` — prepares the board in manual mode.
- `LoadBoardFromFile()` — prepares the board in file mode.
- `GetInput()` — reads the simulation command.
- `Update()` — calculates the next generation.
- `Render()` — displays the current board.
- `Shutdown()` — releases memory.

Two dynamically allocated boards store the current and next generations. After all new cell states are calculated, their pointers are swapped. This prevents changes to one cell from affecting other calculations within the same generation.
