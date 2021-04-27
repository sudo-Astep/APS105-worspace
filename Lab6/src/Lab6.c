#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARDROWS 9
#define BOARDCOLS 11

bool dumpComputer = true; //causes dump of computer board before game if true (for debugging)

// boards. 0 means empty, non-zero means ship is there, number is size, -tive means hit on a ship
int playerBoard[BOARDROWS + 1][BOARDCOLS + 1];
int computerBoard[BOARDROWS + 1][BOARDCOLS + 1];
int compShotBoard[BOARDROWS + 1][BOARDCOLS + 1];

int getRand (int lowval, int highval)
{
	return (rand () % (highval + 1 - lowval) + lowval);
}

int getShot (bool fromUser, int board[BOARDROWS + 1][BOARDCOLS + 1])
{
	int row, col;

	while (1)
	{
		if (fromUser)
		{
			printf ("Give a shot (row, col):");
			scanf ("%d%d", &row, &col);
			if (col < 1 || col > BOARDCOLS || row < 1 || row > BOARDROWS)
				printf ("Invalid input\n");
			else
				//valid input
				break;
		}
		else
		{ //computer generated
			row = getRand (1, BOARDROWS);
			col = getRand (1, BOARDCOLS);
			if (compShotBoard[row][col] == 0)
			{ //make sure haven't shot here before
				compShotBoard[row][col] = 1; //valid shot
				break;
			}
		}
	} //will leave this loop with valid input
	if (board[row][col] != 0)
	{
		if (board[row][col] > 0)
			board[row][col] = -1 * board[row][col]; //make sure is -tive
		return -1 * board[row][col]; //a hit!
	}
	return 0; //miss
}

bool allShipsNotHit (int board[BOARDROWS + 1][BOARDCOLS + 1])
{
	for (int i = 1; i <= BOARDROWS; i++)
	{
		for (int j = 1; j <= BOARDCOLS; j++)
		{
			if (board[i][j] > 0) //ship and not hit
				return (true);
		}
	}
	return (false); //no ships found not all hit
}

bool noneLeft (int valueToFind, int board[BOARDROWS + 1][BOARDCOLS + 1])
{
	for (int i = 1; i <= BOARDROWS; i++)
	{
		for (int j = 1; j <= BOARDCOLS; j++)
		{
			if (board[i][j] == valueToFind) //ship and not hit
				return (false);
		}
	}
	return (true); //no ships found, all hit
}

bool isValidPosition (bool getUserInput, int size, int col, int row,
						int orientation,
						int board[BOARDROWS + 1][BOARDCOLS + 1])
{

	// Column and row must be greater than 0
	if (col < 1 || row < 1)
	{
		if (getUserInput)
			printf ("Invalid Input\n");

		return false;
	}

	// Checks conditions for horizontal orientation
	if (orientation == 0)
	{
		// Checks if the ship goes outside the board on the stern end
		if (col + size > BOARDCOLS + 1)
		{
			// Prints error message if user is inputting
			if (getUserInput)
				printf ("Invalid Input\n");
			return false;
		}

		// Checks if the ship position conflicts with any other ships
		for (int i = col; i < col + size; i++)
		{
			if (board[row][i] != 0)
			{
				// Prints error message is user is inputting
				if (getUserInput)
					printf ("Conflicts with ship already placed\n");
				return false;
			}
		}

		return true;
	}

	// Checks conditions for vertical orientation
	if (orientation == 1)
	{
		// Checks if the ship goes outside the board on the stern end
		if (row + size > BOARDROWS + 1)
		{
			// Prints error message if user is inputting
			if (getUserInput)
				printf ("Invalid Input\n");
			return false;
		}

		// Checks if the ship position conflicts with any other ships
		for (int i = row; i < row + size; i++)
		{
			// Prints error message is user is inputting
			if (board[i][col] != 0)
			{
				if (getUserInput)
					printf ("Conflicts with ship already placed\n");
				return false;
			}
		}

		return true;
	}

	// If the orientation is invalid, error message gets printed
	if (getUserInput)
		printf ("Invalid Input\n");
	return false;
}

void populateBoard (bool getUserInput, int board[BOARDROWS + 1][BOARDCOLS + 1])
{
	// Variable declarations
	int row; // The row of the ship front
	int col; // The column of the ship front
	int orientation; // The orientation of the ship


	// If the user is placing their ships
	if (getUserInput)
	{
		printf ("Rows are 1 - 9, Columns are 1 - 11\n");
		printf ("Orientation is 0 for across, 1 for down\n");

		// Cycles through all the possible ship sizes starting at 5 and down to 1
		for (int size = 5; size > 0; size--)
		{

			// Gets user input and check if it is valid
			do
			{
				printf ("Give starting row, starting column and orientation (3 inputs) for ship of size=%d:",
						size);
				scanf ("%d %d %d", &row, &col, &orientation);
			} while (!isValidPosition (getUserInput, size, col, row,
										orientation, board));

			// Changes the value of the variables in the array according to their position for horizontal orientation
			if (orientation == 0)
			{
				for (int i = col; i < col + size; i++)
				{
					board[row][i] = size;
				}
			}

			// Changes the value of the variables in the array according to their position for vertical orientation
			if (orientation == 1)
			{
				for (int i = row; i < row + size; i++)
				{
					board[i][col] = size;
				}
			}
		}
	}


	// If the computer is placing ships
	else
	{
		// Cycles through all the possible ship sizes starting at 5 and down to 1
		for (int size = 5; size > 0; size--)
		{
			// Chooses random values for the column, row and orientation and makes sure the position in valid
			do
			{
				row = getRand (1, BOARDROWS);
				col = getRand (1, BOARDCOLS);
				orientation = getRand (0, 1);
			} while (!isValidPosition (getUserInput, size, col, row,
										orientation, board));

			// Changes the value of the variables in the array according to their position for horizontal orientation
			if (orientation == 0)
			{
				for (int i = col; i < col + size; i++)
				{
					board[row][i] = size;
				}
			}

			// Changes the value of the variables in the array according to their position for vertical orientation
			if (orientation == 1)
			{
				for (int i = row; i < row + size; i++)
				{
					board[i][col] = size;
				}
			}
		}
	}
}

void showBoard (int board[BOARDROWS + 1][BOARDCOLS + 1])
{
	printf ("   Cols\n");
	printf ("    1  2  3  4  5  6  7  8  9 10 11\n");
	printf ("     ________________________________\n");
	for (int i = 1; i <= BOARDROWS; i++)
	{
		printf ("R%d|", i);
		for (int j = 1; j <= BOARDCOLS; j++)
		{
			if (board[i][j] >= 0)
				printf (" ");
			printf ("%d ", board[i][j]);
		}
		printf ("|\n");
	}
	printf ("  __________________________________\n");
}

int main (int argc, char **argv)
{
	bool playerBoardOK, computerBoardOK;

	// initialize the seed randomly.

	srand (time (NULL));

	//init the boards
	for (int i = 0; i < BOARDROWS; i++)
	{
		for (int j = 0; j < BOARDCOLS; j++)
		{
			playerBoard[i][j] = 0;
			computerBoard[i][j] = 0;
			compShotBoard[i][j] = 0;
		}
	}

	populateBoard (true, playerBoard);
	populateBoard (false, computerBoard);
	printf ("Your board is\n");
	showBoard (playerBoard);
	if (dumpComputer)
	{
		printf ("\n\nComputer board is\n");
		showBoard (computerBoard);
	}

	// play starts here
	playerBoardOK = true;
	computerBoardOK = true;
	while (playerBoardOK && computerBoardOK)
	{
		int hit = getShot (true, computerBoard); //shot from user
		if (hit)
		{
			printf ("HIT on size %d\n", hit);
			if (noneLeft (hit, computerBoard))
				printf ("Sunk computer's %d\n", hit);
		}
		else
			printf ("MISS!!\n");
		hit = getShot (false, playerBoard);
		if (hit)
		{
			printf ("Computer hits! Size=%d\n", hit);
			if (noneLeft (hit, computerBoard))
				printf ("Sunk your %d\n", hit);
		}
		else
			printf ("Computer miss!\n");
		playerBoardOK = allShipsNotHit (playerBoard);
		computerBoardOK = allShipsNotHit (computerBoard);
//    showBoard(playerBoard); //debug
//    showBoard(computerBoard); //debug
	}
	if (playerBoardOK)
		printf ("You win!\n");
	else
		printf ("You lose.\n");
	showBoard (computerBoard);

	return 0;
}
