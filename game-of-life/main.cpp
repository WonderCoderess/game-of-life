#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#define USE_FILE_INPUT 0 // Set to 1 to enable file input, 0 to disable

using namespace std;

bool Initialize(int& rows, int& columns, int**& board, int**& nextBoard);
void PrintRules();
bool GetNumber(int minValue, int maxValue, int& number);
int** CreateBoard(int rows, int columns);
void DeleteBoard(int** board, int rows);
void Render(int** board, int rows, int columns);
bool SetInitialCells(int** board, int rows, int columns);
int CountLiveNeighbours(int** board, int rows, int columns, int row, int column);
int CheckValueNextGen(int** board, int row, int column, int liveNeighbours);
void Update(int**& board, int**& nextBoard, int rows, int columns);
bool GetInput();
bool IsGenerationDead(int** board, int rows, int columns);
bool IsGenerationStable(int** board, int** nextBoard, int rows, int columns);
bool LoadBoardFromFile(int& rows, int& columns, int**& board, int**& nextBoard);
void Shutdown(int** board, int** nextBoard, int rows);

int main()
{
	PrintRules();
	int boardRows = 0;
	int boardColumns = 0;

	int** board = nullptr;
	int** nextBoard = nullptr;

#if USE_FILE_INPUT
	if (!LoadBoardFromFile(boardRows, boardColumns, board, nextBoard))
	{
		return 1;
	}
#else
	if (!Initialize(boardRows, boardColumns, board, nextBoard))
	{
		Shutdown(board, nextBoard, boardRows);
		return 0;
	}
#endif
	cout << endl;
	cout << "Initial generation:" << endl;
	cout << endl;
	Render(board, boardRows, boardColumns);

	while (GetInput())
	{
		Update(board, nextBoard, boardRows, boardColumns);
		cout << endl;
		cout << "Next generation:" << endl;
		cout << endl;
		Render(board, boardRows, boardColumns);
		if (IsGenerationDead(board, boardRows, boardColumns))
		{
			cout << "All cells are dead. Game over." << endl;
			break;
		}
		if (IsGenerationStable(board, nextBoard, boardRows, boardColumns))
		{
			cout << "The generation is stable. Game over." << endl;
			break;
		}
	}

	Shutdown(board, nextBoard, boardRows);

	return 0;
}

// PrintRules displays the rules of Conway's Game of Life to the user.
void PrintRules()
{
	cout << "========================================" << endl;
	cout << "         CONWAY'S GAME OF LIFE" << endl;
	cout << "========================================" << endl;
	cout << endl;

	cout << "Each cell is alive (#) or dead (.)." << endl;
	cout << "Neighbors are the eight surrounding positions," << endl;
	cout << "including diagonals." << endl;
	cout << endl;

	cout << "RULES:" << endl;
	cout << "- A live cell survives with 2 or 3 live neighbors." << endl;
	cout << "- Otherwise, a live cell dies." << endl;
	cout << "- A dead cell becomes alive with exactly 3 live neighbors." << endl;
	cout << "- All other dead cells remain dead." << endl;
	cout << "- All cells update simultaneously." << endl;
	cout << "- Cells outside the board are treated as dead." << endl;
	cout << endl;

	cout << "INITIAL BOARD SETUP:" << endl;
	cout << "The program supports two input modes:" << endl;
	cout << "- File input: load the board from board.txt." << endl;
	cout << "- Manual input: choose the board size and live cells." << endl;
	cout << "The input mode is selected before compilation." << endl;
	cout << endl;

#if USE_FILE_INPUT
	cout << "FILE INPUT:" << endl;
	cout << "The initial board is loaded from board.txt." << endl;
	cout << "The first two numbers specify rows and columns." << endl;
	cout << "The remaining values describe cells: 0 = dead, 1 = alive." << endl;
#else
	cout << "MANUAL INPUT:" << endl;
	cout << "1. Choose the number of rows and columns (1-50)." << endl;
	cout << "2. Select live cells one at a time." << endl;
	cout << "3. Enter the row first, then the column, starting from 1." << endl;
	cout << "4. Enter 0 when asked for a row to finish setup." << endl;
	cout << "All unselected cells remain dead." << endl;
#endif
	cout << endl;
}

// Initialize prompts the user for the number of rows and columns, creates the board and nextBoard, and sets the initial live cells.
bool Initialize(int& rows, int& columns,
	int**& board, int**& nextBoard)
{
	cout << "Choose number of rows." << endl;

	if (!GetNumber(1, 50, rows))
	{
		return false;
	}

	cout << "Choose number of columns." << endl;

	if (!GetNumber(1, 50, columns))
	{
		return false;
	}

	board = CreateBoard(rows, columns);
	nextBoard = CreateBoard(rows, columns);

	return SetInitialCells(board, rows, columns);
}

// GetNumber prompts the user for a number within a specified range and validates the input.
bool GetNumber(int minValue, int maxValue, int& number)
{
	while (true)
	{
		cout << "Enter a number (" << minValue
			<< "-" << maxValue << "): ";
		
		string line;

		if (!getline(cin, line))
		{
			cout << endl << "Input ended." << endl;
			return false;
		}

		istringstream input(line);
		int value = 0;
		char extra;

		if (!(input >> value) ||
			(input >> extra) ||
			value < minValue || value > maxValue)
		{
			cout << "Invalid value. Try again." << endl;
		}
		else
		{
			number = value;
			return true;
		}
	}
}

// CreateBoard allocates memory for a 2D array (board) and initializes all cells to 0 (dead).
int** CreateBoard(int rows, int columns)
{
	int** board = new int* [rows];

	for (int row = 0; row < rows; row++)
	{
		board[row] = new int[columns];

		for (int column = 0; column < columns; column++)
		{
			board[row][column] = 0;
		}
	}

	return board;
}

// DeleteBoard deallocates the memory used for the board.
void DeleteBoard(int** board, int rows)
{
	if (board == nullptr)
	{
		return;
	}

	for (int row = 0; row < rows; row++)
	{
		delete[] board[row];
	}

	delete[] board;
}

// Render displays the board on the console, using '#' for live cells and '.' for dead cells.
void Render(int** board, int rows, int columns)
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			if (board[row][column] == 1)
			{
				cout << "# ";
			}
			else
			{
				cout << ". ";
			}
		}

		cout << endl;
		cout << endl;
	}
}

// SetInitialCells prompts the user to set the initial live cells on the board, allowing them to specify the row and column for each live cell.
bool SetInitialCells(int** board, int rows, int columns)
{
	cout << endl;
	cout << "Set the initial live cells, one at a time." << endl;
	cout << "Enter the row first, then the column." << endl;
	cout << "Enter 0 when asked for a row to finish." << endl;
	cout << endl;

	while (true)
	{
		cout << "Choose row (0 to finish)." << endl;
		int row = 0;

		if (!GetNumber(0, rows, row))
		{
			return false;
		}

		if (row == 0)
		{
			return true;
		}

		cout << "Choose column." << endl;
		int column = 0;

		if (!GetNumber(1, columns, column))
		{
			return false;
		}

		board[row - 1][column - 1] = 1;

		cout << "Cell at (" << row << ", " << column
			<< ") is now alive." << endl;
		cout << endl;
	}
}

// Counts live neighbors, including diagonals, excluding the cell itself and positions outside the board.
int CountLiveNeighbours(int** board, int rows, int columns, int row, int column)
{
	int count = 0;

	for (int rowOffset = -1; rowOffset <= 1; rowOffset++)
	{
		for (int columnOffset = -1; columnOffset <= 1; columnOffset++)
		{
			// skip the cell itself
			if (rowOffset == 0 && columnOffset == 0)
			{
				continue;
			}

			int neighbourRow = row + rowOffset;
			int neighbourColumn = column + columnOffset;

			if (neighbourRow >= 0 && neighbourRow < rows &&
				neighbourColumn >= 0 && neighbourColumn < columns)
			{
				if (board[neighbourRow][neighbourColumn] == 1)
				{
					count++;
				}
			}
		}
	}

	return count;
}

// CheckValueNextGen determines the next state of a cell based on its current state and the number of live neighbors.
int CheckValueNextGen(int** board, int row, int column, int liveNeighbours)
{
	int valueNextGen = 0;

	if (board[row][column] == 1) 
	{
		if (liveNeighbours == 2 || liveNeighbours == 3)
		{
			valueNextGen = 1;
		}
	}

	if (board[row][column] == 0) 
	{
		if (liveNeighbours == 3)
		{
			valueNextGen = 1;
		}
	}
	return valueNextGen;
}

// Update applies the rules of Conway's Game of Life to update the board for the next generation.
void Update(int**& board, int**& nextBoard, int rows, int columns)
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			int liveNeighbours = CountLiveNeighbours(board, rows, columns, row, column);
			nextBoard[row][column] = CheckValueNextGen(board, row, column, liveNeighbours);
		}
	}
	int** temporary = board; // Swap the pointers to avoid copying the entire board
	board = nextBoard;
	nextBoard = temporary;
}

// GetInput prompts the user to choose between proceeding to the next generation or quitting the game.
bool GetInput()
{
	cout << "1 - next generation, 0 - quit" << endl;

	int choice = 0;

	if (!GetNumber(0, 1, choice))
	{
		return false;
	}

	return choice == 1;
}

// IsGenerationDead checks if all cells in the current generation are dead (0).
bool IsGenerationDead(int** board, int rows, int columns)
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			if (board[row][column] == 1)
			{
				return false;
			}
		}
	}
	return true;
}

// IsGenerationStable checks if the current generation is the same as the previous generation, indicating stability.
bool IsGenerationStable(int** board, int** nextBoard,
	int rows, int columns)
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			if (board[row][column] != nextBoard[row][column])
			{
				return false;
			}
		}
	}

	return true;
}

// Shutdown deallocates the memory used for both the current and next generation boards.
void Shutdown(int** board, int** nextBoard, int rows)
{
	DeleteBoard(board, rows);
	DeleteBoard(nextBoard, rows);
}

// LoadBoardFromFile reads the board dimensions and cell values from a file named "board.txt" and initializes the board and nextBoard accordingly.
bool LoadBoardFromFile(int& rows, int& columns, int**& board, int**& nextBoard)
{
	ifstream file("board.txt");

	if (!file.is_open())
	{
		cout << "Could not open board.txt." << endl;
		return false;
	}

	if (!(file >> rows >> columns))
	{
		cout << "Could not read board dimensions." << endl;
		return false;
	}

	if (rows < 1 || rows > 50 || columns < 1 || columns > 50)
	{
		cout << "Board dimensions must be between 1 and 50." << endl;
		return false;
	}

	cout << "Dimensions from file: "
		<< rows << " x " << columns << endl;

	board = CreateBoard(rows, columns);
	nextBoard = CreateBoard(rows, columns);

	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			int value = 0;

			if (!(file >> value) || (value != 0 && value != 1))
			{
				cout << "Invalid or missing cell value." << endl;

				DeleteBoard(board, rows);
				DeleteBoard(nextBoard, rows);
				board = nullptr;
				nextBoard = nullptr;

				return false;
			}

			board[row][column] = value;
		}
	}

	char extra; 

	if (file >> extra)
	{
		cout << "Unexpected data after the board." << endl; // Check for any extra data after the expected board values ex. 1 1 1abc

		DeleteBoard(board, rows);
		DeleteBoard(nextBoard, rows);
		board = nullptr;
		nextBoard = nullptr;

		return false;
	}

	return true;
}