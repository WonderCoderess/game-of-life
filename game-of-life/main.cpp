#include <iostream>
#include <cstdlib>
using namespace std;

void Initialize(int& rows, int& columns,
	int**& board, int**& nextBoard);

int GetNumber(int minValue, int maxValue);
int** CreateBoard(int rows, int columns);
void DeleteBoard(int** board, int rows);
void Render(int** board, int rows, int columns);

void SetInitialCells(int** board, int rows, int columns);

int CountLiveNeighbors(int** board, int rows, int columns,
	int row, int column);

void Update(int** board, int** nextBoard, int rows, int columns);

int main()
{
	int boardRows = 0;
	int boardColumns = 0;

	int** board = nullptr;
	int** nextBoard = nullptr;

	Initialize(boardRows, boardColumns, board, nextBoard);

	cout << "Board size: " << boardRows << " x "
		<< boardColumns << endl;

	Render(board, boardRows, boardColumns);

	cout << "Neighbours of center" << CountLiveNeighbors(board, boardColumns, boardRows, 1, 1) << endl;
	cout << "Neighbours of left, right corner" << CountLiveNeighbors(board, boardColumns, boardRows, 0, 0) << endl;

	DeleteBoard(board, boardRows);
	DeleteBoard(nextBoard, boardRows);

	return 0;
}

// Initialize prompts the user for the board size, creates the board and nextBoard, and sets the initial live cells.
void Initialize(int& rows, int& columns,
	int**& board, int**& nextBoard) {
	cout << "Choose number of rows." << endl;
	rows = GetNumber(1, 50);

	cout << "Choose number of columns." << endl;
	columns = GetNumber(1, 50);

	board = CreateBoard(rows, columns);
	nextBoard = CreateBoard(rows, columns);

	SetInitialCells(board, rows, columns);

}

// prompts the user for a number between minValue and maxValue, inclusive.
int GetNumber(int minValue, int maxValue)
{
	int number = 0;

	while (true)
	{
		cout << "Enter a number (" << minValue
			<< "-" << maxValue << "): ";
		cin >> number;

		if (cin.eof())
		{
			cout << endl << "Input ended." << endl;
			exit(1);
		}

		if (cin.fail() || number < minValue || number > maxValue)
		{
			cout << "Invalid value. Try again." << endl;
			cin.clear();
			cin.ignore(1000, '\n');
		}
		else
		{
			cin.ignore(1000, '\n');
			return number;
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
	}
}

// SetInitialCells prompts the user to set the initial live cells on the board.
void SetInitialCells(int** board, int rows, int columns)
{
	while (true)
	{
		cout << "Choose row (0 to finish)." << endl;
		int row = GetNumber(0, rows);

		if (row == 0)
		{
			break;
		}

		cout << "Choose column." << endl;
		int column = GetNumber(1, columns);

		board[row - 1][column - 1] = 1;
	}
}

// Counts live neighbors, including diagonals, excluding the cell itself and positions outside the board.
int CountLiveNeighbors(int** board, int rows, int columns,
	int row, int column)
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

			int neighborRow = row + rowOffset;
			int neighborColumn = column + columnOffset;

			if (neighborRow >= 0 && neighborRow < rows &&
				neighborColumn >= 0 && neighborColumn < columns)
			{
				if (board[neighborRow][neighborColumn] == 1)
				{
					count++;
				}
			}
		}
	}

	return count;
}

// Update applies the rules of Conway's Game of Life to update the board for the next generation.
void Update(int** board, int** nextBoard, int rows, int columns)
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			int liveNeighbors = CountLiveNeighbors(board, rows, columns, row, column);
		}
	}
}

int CheckValueNextGen(int** board, int row, int column, int liveNeighbours)
{
	/* 
	ustawiam na 0 /mawrtwa
	
	if zywa - using '#' for live cells and '.'
	 0|| 1 - martwa /nie obsluguje
	 2 || 3 - zywa
	 4 || 8 - martw / nie obsluguje
	 if martwa
	 3 - zywa
	 other martwa / nie obsluguje
	*/
	int valueNextGen = 0;

	if (board[row][column] == 1) {
		if (liveNeighbours == 2 || liveNeighbours == 3)
		{
			valueNextGen = 1;
		}
	}


	if (board[row][column] == 0) {
		if (liveNeighbours == 3)
		{
			valueNextGen = 1;
		}
	}
	return valueNextGen;
}