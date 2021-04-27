/*
 * File:        project_reversi_skeleton.c
 * Author:      APS105H1 Teaching Team
 * Modified by: Andrija Stepanovic
 *
 * Date: Jan 2021
 */

#include "project_reversi_skeleton.h" // DO NOT modify this line
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// =========   Function declaration  ==========

bool isLegalMove (char[], char[][3], int);
void changeBoard (char[][26], char[], int);
void changeBoardInDirection (char[][26], int, int, int, char, int, int);
void checkPossibleMoves (char[][26], int, int, int, char (*whiteMoves)[3], char (*blackMoves)[3]);
bool checkLegal (char[][26], int, int, int, char);

#ifndef COMPETE_MODE // DO NOT DELETE THIS LINE
int main (void)
{
	// Variable Declarations
	int boardSize; // Board size
	char input[4]; // Input String, we know that user input will only ever be a short String
	size_t maxLinesWhite = 10; // Variable that dictates the size of the whiteMoves dynamic array, so it doesn't have to be reallocated constantly (as in every time something is added)
	size_t maxLinesBlack = 10; // variable that dictates the size of the blackMoves dynamic array
	char (*whiteMoves)[3] = calloc (maxLinesWhite, sizeof *whiteMoves); // Array that keeps all the possible moves of the white player
	char (*blackMoves)[3] = calloc (maxLinesBlack, sizeof *blackMoves); // Array that keeps all the possible moves of the black player
	char board[26][26]; // Board that the game is played on

	// User input for board size
	printf ("Enter the board dimension: ");
	scanf ("%d", &boardSize);

	// Initialization of empty board
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			*(*(board + i) + j) = 'U';
		}
	}

	// Initial board position with white and black pieces in a cross
	board[(int) ((boardSize / 2.0) - 0.5)][(int) ((boardSize / 2.0) - 0.5)] = 'W';
	board[(int) ((boardSize / 2.0) + 0.5)][(int) ((boardSize / 2.0) + 0.5)] = 'W';
	board[(int) ((boardSize / 2.0) + 0.5)][(int) ((boardSize / 2.0) - 0.5)] = 'B';
	board[(int) ((boardSize / 2.0) - 0.5)][(int) ((boardSize / 2.0) + 0.5)] = 'B';

	printBoard (board, boardSize);

	printf ("Enter board configuration:\n");

	// User inputs the initial conditions of the board
	while (strcmp (input, "!!!") != 0)
	{
		scanf ("%s", input);

		// Places the pieces in their place
		board[(int) *(input + 1) - 97][(int) *(input + 2) - 97] = *(input);
	}

	printBoard (board, boardSize);

	// Checks all possible moves and fills the whiteMoves and blackMoves arrays with them
	checkPossibleMoves (board, boardSize, maxLinesWhite, maxLinesBlack, whiteMoves, blackMoves);

	printf ("Available moves for W:\n");

	// Prints all available moves for white player
	for (int i = 0; i < maxLinesWhite; i++)
	{
		if (strcmp (*(whiteMoves + i), "") == 0)
			break;

		printf ("%s\n", whiteMoves[i]);
	}

	printf ("Available moves for B:\n");

	// Prints all available moves for black player
	for (int i = 0; i < maxLinesBlack; i++)
	{
		if (strcmp (*(blackMoves + i), "") == 0)
			break;

		printf ("%s\n", blackMoves[i]);
	}

	printf("Enter a move:\n");
	// Gets player input for next move
	scanf ("%s", input);

	// Males a temporary string for checking if the move is legal
	char temp[3] =
	{ input[1], input[2], '\0' };

	// Checks if move is legal for white player
	if (input[0] == 'W')
	{
		if (isLegalMove (temp, whiteMoves, maxLinesWhite))
		{
			printf ("Valid move.\n");

			// Changes board to reflect move
			changeBoard (board, input, boardSize);

			// Places legal piece on the board
			board[(int) *(input + 1) - 97][(int) *(input + 2) - 97] = *(input);
		}
		else
			printf ("Invalid move.\n");
	}

	// Checks if move is legal for vlack player
	if (input[0] == 'B')
	{
		if (isLegalMove (temp, blackMoves, maxLinesBlack))
		{
			printf ("Valid move.\n");

			// Changes board to reflect moves
			changeBoard (board, input, boardSize);

			// Places piece on the board
			board[(int) *(input + 1) - 97][(int) *(input + 2) - 97] = *(input);
		}
		else
			printf ("Invalid move.\n");
	}

	// Prints final board arrangement
	printBoard (board, boardSize);

	return 0;
}
#endif // DO NOT DELETE THIS LINE

// ========= Function implementation ==========

/*
 * Function: changeBoard
 * ------------------------
 * Changes the board configuration according to the given configuration and move
 */
void changeBoard (char board[][26], char move[], int n)
{
	char colour = move[0]; //Player color
	int row = move[1] - 97; //row where piece is places
	int col = move[2] - 97; //column where piece is placed

	//checks for legality in all directions and then changes the board in the directions that are legal
	for (int deltaRow = -1; deltaRow <= 1; deltaRow++)
	{
		for (int deltaCol = -1; deltaCol <= 1; deltaCol++)
		{
			if (checkLegalInDirection (board, n, row, col, colour, deltaRow, deltaCol))
			{
				changeBoardInDirection (board, n, row, col, colour, deltaRow, deltaCol);
			}
		}
	}
}

/*
 * Function: changeBoardInDirection
 * ---------------------------
 * Changes the color of the pieces in the given direction util it hits the end of a piece of the player's color
 */
void changeBoardInDirection (char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol)
{
	char oppositeColour; // Color opposite of the player

	if (colour == 'W')
		oppositeColour = 'B';
	else
		oppositeColour = 'W';

	// Goes in a given direction and changes the color until it hits a stopping point
	for (int i = 1; i < n; i++)
	{
		int currentRow = row + (i * deltaRow);
		int currentCol = col + (i * deltaCol);

		if (!positionInBounds (n, currentRow, currentCol))
			break;

		if (board[currentRow][currentCol] == oppositeColour)
			board[currentRow][currentCol] = colour;
		else
			break;
	}
}

/*
 * Function: isLegalMove
 * -----------------
 * Checks if the given move is legal
 */
bool isLegalMove (char move[], char availableMoves[][3], int maxMoves)
{
	printf("%s ", move);

	// Checks if the move is in the array of all available moves
	for (int i = 0; i <= maxMoves; i++)
	{
		printf("%s ", availableMoves[i]);
		printf("%d\n", strcmp (move, availableMoves[i]));

		if (strcmp (move, availableMoves[i]) == 0)
			return true;
	}

	return false;
}

/*
 * Function:  printBoard
 * --------------------
 * Print the whole board to the terminal window.
 */
void printBoard (char board[][26], int n)
{
	printf ("  ");

	// Prints the top bar starting from a to z
	for (int i = 0; i < n; i++)
		printf ("%c", (char) (97 + i));

	printf ("\n");

	// Prints the board
	for (int i = 0; i < n; i++)
	{
		printf ("%c ", (char) (97 + i));
		for (int j = 0; j < n; j++)
		{
			printf ("%c", *(*(board + i) + j));
		}

		printf ("\n");
	}
}

/*
 * Function:  positionInBounds
 * --------------------
 * Checks whether the specified (row, col) lies within the board dimensions.
 */
bool positionInBounds (int n, int row, int col)
{
	if ((row >= n || col >= n) || (row < 0 || col < 0))
		return false;

	return true;
}

/*
 * Function:  checkLegalInDirection
 * --------------------
 * Checks whether (row, col) is a legal position for a tile of colour by “looking” in the direction
 * specified by deltaRow and deltaCol.
 */
bool checkLegalInDirection (char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol)
{
	char oppositeColour; //Color opposite of the current player

	if (colour == 'W')
		oppositeColour = 'B';
	else
		oppositeColour = 'W';

//	if (row + deltaRow == n || col + deltaCol == n)
//		return false;
//
//	if (board[row + deltaRow][col + deltaCol] == oppositeColour && board[row + 2 * deltaRow][col + 2 * deltaCol] == colour)
//		return true;
//	else if (board[row + deltaRow][col + deltaCol] == oppositeColour)
//		return checkLegalInDirection (board, n, row + deltaRow, col + deltaCol, colour, deltaRow, deltaCol);
//	else
//		return false;

	// Checks legality in the given direction
	for (int i = 1; i < n; i++)
	{
		int currentRow = row + (i * deltaRow);
		int currentCol = col + (i * deltaCol);

		// False if goes out of bounds without hitting a true
		if (!positionInBounds(n, currentRow + deltaRow, currentCol + deltaCol))
			return false;

		// If the player color is hit and all previous spots were opposite, return true
		if (board[currentRow][currentCol] == oppositeColour && board[currentRow + deltaRow][currentCol + deltaCol] == colour)
			return true;
		else if (board[currentRow][currentCol] == oppositeColour)
			continue;
		else
			break;
	}

	return false;
}

/*
 * Function:  makeMove
 * --------------------
 * The player "turn" makes a move at (row, col).
 * Note: This function MUST NOT change board, only row and col can be changed to reflect the move.
 */
int makeMove (const char board[26][26], int n, char turn, int *row, int *col)
{
	(void) board;
	(void) n;
	(void) turn;
	(void) row;
	(void) col;
	return 0;
}

bool checkLegal (char board[][26], int n, int row, int col, char color)
{
	for (int deltaRow = -1; deltaRow <= 1; deltaRow++)
	{
		for (int deltaCol = -1; deltaCol <= 1; deltaCol++)
		{
			if (checkLegalInDirection (board, n, row, col, color, deltaRow, deltaCol))
				return true;
		}
	}

	return false;
}

/*
 * Function: checkPossibleMoves
 * -----------------------------
 * Finds all possible legal moves for both players and puts them into a dynamic array
 */
void checkPossibleMoves (char board[][26], int n, int maxLinesWhite, int maxLinesBlack, char (*whiteMoves)[3], char (*blackMoves)[3])
{
	int numWhiteMoves = 0; // Counter for the number of white moves
	int numBlackMoves = 0; // Counter for the number of black moves

	// Goes through each position on the board and each direction for every position
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			if (board[row][col] == 'U')
			{
				// If a move is legal in a given direction, it is put into the array of correct moves
				if (checkLegal (board, n, row, col, 'W'))
				{
					// If the array goes over it's allocated size it gets expanded
					if (numWhiteMoves > maxLinesWhite)
					{
						maxLinesWhite *= 2;
						char (*temp)[3] = realloc (whiteMoves, sizeof *whiteMoves * maxLinesWhite);
						whiteMoves = temp;
					}

					// Temporary variable representing the current move
					char temp[3] =
					{ (char) 97 + row, (char) 97 + col, '\0' };

					memcpy (whiteMoves[numWhiteMoves], temp, sizeof *temp * 3);

					numWhiteMoves++;
				}

				// If a move is legal in a given direction, it is put into the array of correct moves
				if (checkLegal (board, n, row, col, 'B'))
				{
					// If the array goes over it's allocated size it gets expanded
					if (numBlackMoves > maxLinesBlack)
					{
						maxLinesBlack *= 2;
						char (*temp)[3] = realloc (blackMoves, sizeof *blackMoves * maxLinesBlack);
						blackMoves = temp;
					}

					// Temporary variable representing the current move
					char temp[3] =
					{ (char) 97 + row, (char) 97 + col, '\0' };

					memcpy (blackMoves[numBlackMoves], temp, sizeof *temp * 3);

					numBlackMoves++;
				}
			}
		}
	}
}

