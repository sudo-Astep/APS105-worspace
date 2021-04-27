/*
 * File:        project_reversi_skeleton.c
 * Author:      APS105H1 Teaching Team
 * Modified by: Andrija Stepanovic
 *
 * Date: Jan 2021
 */

#include "project_reversi_skeleton.h" // DO NOT modify this line
#include "lab8part2lib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// =========   Function declaration  ==========

bool isLegalMove (char[], char[][3], int);
void changeBoard (char[][26], char[], char, int);
void changeBoardInDirection (char[][26], int, int, int, char, int, int);
void checkPossibleMoves (char[][26], int, size_t*, char (*moves)[3], char, bool*);
bool isGameOver (bool, bool);
//int piecesFlipped (char[][26], char[], char, int);
//int piecesFlippedInDirection (char[][26], int, int, int, char, int, int);
void resetMoves (char (*whiteMoves)[3], char (*blackMoves)[3], bool*, bool*, size_t, size_t);
char findWinner (char[][26], int);
bool isCornerMove (char[], int);
int nextMove (const char[][26], int, char, char[]);
bool checkLegal (char[][26], int, int, int, char);

#ifndef COMPETE_MODE // DO NOT DELETE THIS LINE
int main (void)
{
	// Variable Declarations
	int n; // Board size
	size_t maxLinesWhite = 512; // Variable that dictates the size of the whiteMoves dynamic array, so it doesn't have to be reallocated constantly (as in every time something is added)
	size_t maxLinesBlack = 512; // variable that dictates the size of the blackMoves dynamic array
	char (*whiteMoves)[3] = calloc (maxLinesWhite, sizeof *whiteMoves); // Array that keeps all the possible moves of the white player
	char (*blackMoves)[3] = calloc (maxLinesBlack, sizeof *blackMoves); // Array that keeps all the possible moves of the black player
	char board[26][26]; // Board that the game is played on
	char computerColor; // Computer piece color
	bool whiteHasMoves = false;
	bool blackHasMoves = false;

	resetMoves (whiteMoves, blackMoves, &whiteHasMoves, &blackHasMoves, maxLinesWhite, maxLinesBlack);

	// User input for board size
	printf ("Enter the board dimension: ");
	scanf ("%d", &n);

	// Initialization of empty board
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			*(*(board + i) + j) = 'U';
		}
	}

	// Initial board position with white and black pieces in a cross
	board[(int) ((n / 2.0) - 0.5)][(int) ((n / 2.0) - 0.5)] = 'W';
	board[(int) ((n / 2.0) + 0.5)][(int) ((n / 2.0) + 0.5)] = 'W';
	board[(int) ((n / 2.0) + 0.5)][(int) ((n / 2.0) - 0.5)] = 'B';
	board[(int) ((n / 2.0) - 0.5)][(int) ((n / 2.0) + 0.5)] = 'B';

	printf ("Computer plays (B/W) : ");
	scanf (" %c", &computerColor);

	printBoard (board, n);

	checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
	checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);

	int row = 0;
	int col = 0;

	while (!isGameOver (whiteHasMoves, blackHasMoves))
	{
		if (computerColor == 'B')
		{
			if (blackHasMoves)
			{
				printf("%s\n", blackMoves[0]);

				printf ("%d\n",makeMove (board, n, computerColor, &row, &col));

				printf ("%d %d\n", row, col);

				char computerMove[3] =
				{ 97 + row, 97 + col, '\0' };

				printf ("Computer places %c at %s.\n", computerColor, computerMove);

				changeBoard (board, computerMove, computerColor, n);

				printBoard (board, n);

			}
			else
			{
				printf ("B player has no available moves\n");
			}

			resetMoves (whiteMoves, blackMoves, &whiteHasMoves, &blackHasMoves, maxLinesWhite, maxLinesBlack);

			checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
			checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);

			if (whiteHasMoves)
			{
				findSmartestMove(board, n, 'W', &row, &col);
				printf("Testing AI move (row, col): %c%c\n", row + 'a', col +'a');

				char playerMove[3] =
				{ 97 + row, 97 + col, '\0' };

				if (isLegalMove (playerMove, whiteMoves, maxLinesWhite))
				{
					changeBoard (board, playerMove, 'W', n);

					printBoard (board, n);
				}
				else
				{
					printf ("Invalid move.\n");
					printf ("B player wins.");
					return 0;
				}
			}
			else if (!isGameOver (whiteHasMoves, blackHasMoves))
			{
				printf ("W player has no valid move.\n");
			}

			resetMoves (whiteMoves, blackMoves, &whiteHasMoves, &blackHasMoves, maxLinesWhite, maxLinesBlack);

			checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
			checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);
		}

		if (computerColor == 'W')
		{
			if (blackHasMoves)
			{
				findSmartestMove(board, n, 'B', &row, &col);
				printf("Testing AI move (row, col): %c%c\n", row + 'a', col +'a');

				char playerMove[3] =
				{ 97 + row, 97 + col, '\0' };

				if (isLegalMove (playerMove, blackMoves, maxLinesBlack))
				{
					changeBoard (board, playerMove, 'B', n);
					printBoard (board, n);
				}
				else
				{
					printf ("Invalid move.\n");
					printf ("W player wins.\n");
					return 0;
				}
			}
			else
			{
				printf ("B player has no valid move.\n");
			}

			resetMoves (whiteMoves, blackMoves, &whiteHasMoves, &blackHasMoves, maxLinesWhite, maxLinesBlack);

			checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
			checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);

			if (whiteHasMoves)
			{
				printf("%s\n", whiteMoves[0]);

				printf ("%d\n",makeMove (board, n, computerColor, &row, &col));

				printf ("%d %d\n", row, col);

				char computerMove[3] =
				{ 97 + row, 97 + col, '\0' };

				printf ("Computer places %c at %s.\n", computerColor, computerMove);

				changeBoard (board, computerMove, computerColor, n);

				printBoard (board, n);
			}
			else if (!isGameOver (whiteHasMoves, blackHasMoves))
			{
				printf ("W player has no valid move.\n");
			}

			resetMoves (whiteMoves, blackMoves, &whiteHasMoves, &blackHasMoves, maxLinesWhite, maxLinesBlack);

			checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
			checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);

			if (!blackHasMoves && !whiteHasMoves)
				printf("GameOver");
		}
	}

	char winner;

	winner = findWinner (board, n);

	printf ("%c player wins.", winner);

	return 0;
}
#endif // DO NOT DELETE THIS LINE

// ========= Function implementation ==========

char findWinner (char board[][26], int n)
{
	int whiteScore = 0;
	int blackScore = 0;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (board[i][j] == 'W')
				whiteScore++;

			if (board[i][j] == 'B')
				blackScore++;
		}
	}

	if (whiteScore > blackScore)
		return 'W';
	else
		return 'B';
}

bool isGameOver (bool whiteHasMoves, bool blackHasMoves)
{
	if (!(whiteHasMoves || blackHasMoves))
	{
		return true;
	}

	return false;
}

void resetMoves (char (*whiteMoves)[3], char (*blackMoves)[3], bool *whiteHasMoves, bool *blackHasMoves, size_t maxLinesWhite, size_t maxLinesBlack)
{
	char nullMove[3] =
	{ '0', '0', '\0' };

	char (*newWhiteMoves)[3] = calloc (maxLinesWhite, sizeof *whiteMoves); // Array that keeps all the possible moves of the white player
	char (*newBlackMoves)[3] = calloc (maxLinesBlack, sizeof *blackMoves); // Array that keeps all the possible moves of the black player

	strcpy (newWhiteMoves[0], nullMove);
	strcpy (newBlackMoves[0], nullMove);

	blackMoves = newBlackMoves;
	whiteMoves = newWhiteMoves;

	*whiteHasMoves = false;
	*blackHasMoves = false;
}

/*
 * Function: changeBoard
 * ------------------------
 * Changes the board configuration according to the given configuration and move
 */
void changeBoard (char board[][26], char move[], char colour, int n)
{
	int row = move[0] - 97; //row where piece is places
	int col = move[1] - 97; //column where piece is placed

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

	board[row][col] = colour;
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
	// Checks if the move is in the array of all available moves
	for (int i = 0; i <= maxMoves; i++)
	{
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
			printf ("%c", board[i][j]);
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
 * Function:  makeMove
 * --------------------
 * The player "turn" makes a move at (row, col).
 * Note: This function MUST NOT change board, only row and col can be changed to reflect the move.
 */
int makeMove (const char board[26][26], int n, char turn, int *row, int *col)
{
	size_t maxLines = 512;
	char (*moves)[3] = calloc (maxLines, sizeof *moves);
	char tempBoard[26][26];
	bool hasMoves = false;
	char oppositeTurn;

	if (turn == 'W')
		oppositeTurn = 'B';
	else
		oppositeTurn = 'W';

	for (int i = 0; i < 26; i++)
	{
		strcpy (tempBoard[i], board[i]);
	}

	checkPossibleMoves (tempBoard, n, &maxLines, moves, turn, &hasMoves);

	char (*firstPriority)[3] = calloc (maxLines, sizeof *firstPriority);
	char (*secondPriority)[3] = calloc (maxLines, sizeof *secondPriority);
	char (*thirdPriority)[3] = calloc (maxLines, sizeof *thirdPriority);
	char (*fourthPriority)[3] = calloc (maxLines, sizeof *fourthPriority);
	char (*fifthPriority)[3] = calloc (maxLines, sizeof *fifthPriority);

	bool areaHasMoves[5] =
	{ false, false, false, false, false };

	int counters[5] =
	{ 0, 0, 0, 0, 0 };

	char curMove[3];

	for (int i = 0; i < maxLines; i++)
	{
		strcpy (curMove, moves[i]);

		if (isCornerMove (curMove, n))
		{
			strcpy (firstPriority[counters[0]], curMove);
			counters[0]++;
			areaHasMoves[0] = true;
		}

		for (int j = 2; j < n - 2; j++)
		{
			if (curMove[0] - 97 == j)
			{
				if (curMove[1] - 97 == 0 || curMove[1] - 97 == n - 1)
				{
					strcpy (secondPriority[counters[1]], curMove);
					counters[1]++;
					areaHasMoves[1] = true;
				}
			}

			if (curMove[0] - 97 == 0 || curMove[0] - 97 == n - 1)
			{
				if (curMove[1] - 97 == j)
				{
					strcpy (secondPriority[counters[1]], curMove);
					counters[1]++;
					areaHasMoves[1] = true;
				}
			}
		}

		for (int j = 2; j < n - 2; j++)
		{
			for (int k = 2; k < n - 2; k++)
			{
				if (curMove[0] - 97 == j && curMove[1] - 97 == k)
				{
					strcpy (thirdPriority[counters[2]], curMove);
					counters[2]++;
					areaHasMoves[2] = true;
				}
			}
		}

		for (int j = 2; j < n - 2; j++)
		{
			if (curMove[0] - 97 == j)
			{
				if (curMove[1] - 97 == 1 || curMove[1] - 97 == n - 2)
				{
					strcpy (fourthPriority[counters[3]], curMove);
					counters[3]++;
					areaHasMoves[3] = true;
				}
			}

			if (curMove[0] - 97 == 1 || curMove[0] - 97 == n - 2)
			{
				if (curMove[1] - 97 == j)
				{
					strcpy (fourthPriority[counters[3]], curMove);
					counters[3]++;
					areaHasMoves[3] = true;
				}
			}
		}

		if (curMove[0] - 97 == 0)
		{
			if (curMove[1] - 97 == 1 || curMove[1] - 97 == n - 2)
			{
				if (board[0][0] == turn || board[0][n - 1] == turn)
				{
					strcpy (secondPriority[counters[1]], curMove);
					counters[1]++;
					areaHasMoves[1] = true;
				}
				else
				{
					strcpy (fifthPriority[counters[4]], curMove);
					counters[4]++;
					areaHasMoves[4] = true;
				}
			}
		}

		if (curMove[0] - 97 == 1)
		{
			if (curMove[1] - 97 == 1 || curMove[1] - 97 == n - 2 || curMove[1] - 97 == 0 || curMove[1] - 97 == n - 1)
			{
				if (board[0][0] == turn || board[0][n - 1] == turn)
				{
					strcpy (secondPriority[counters[1]], curMove);
					counters[1]++;
					areaHasMoves[1] = true;
				}
				else
				{
					strcpy (fifthPriority[counters[4]], curMove);
					counters[4]++;
					areaHasMoves[4] = true;
				}
			}
		}

		if (curMove[0] - 97 == n - 1)
		{
			if (curMove[1] - 97 == 1 || curMove[1] - 97 == n - 2)
			{
				if (board[n - 1][0] == turn || board[n - 1][n - 1] == turn)
				{
					strcpy (secondPriority[counters[1]], curMove);
					counters[1]++;
					areaHasMoves[1] = true;
				}
				else
				{
					strcpy (fifthPriority[counters[4]], curMove);
					counters[4]++;
					areaHasMoves[4] = true;
				}
			}
		}

		if (curMove[0] - 97 == n - 2)
		{
			if (curMove[1] - 97 == 1 || curMove[1] - 97 == n - 2 || curMove[1] - 97 == 0 || curMove[1] - 97 == n - 1)
			{
				if (board[n - 1][0] == turn || board[n - 1][n - 1] == turn)
				{
					strcpy (secondPriority[counters[1]], curMove);
					counters[1]++;
					areaHasMoves[1] = true;
				}
				else
				{
					strcpy (fifthPriority[counters[4]], curMove);
					counters[4]++;
					areaHasMoves[4] = true;
				}
			}
		}
	}

	int maxArea = 0;
	int maxAreaIndex = 0;

	if (areaHasMoves[0])
	{
		for (int i = 0; i < maxLines; i++)
		{
			int moveArea = nextMove (tempBoard, n, oppositeTurn, firstPriority[i]);

			if (moveArea > maxArea)
			{
				maxArea = moveArea;
				maxAreaIndex = i;
			}
		}

		*row = firstPriority[maxAreaIndex][0] - 97;
		*col = firstPriority[maxAreaIndex][1] - 97;
		return 1;
	}

	if (areaHasMoves[1])
	{
		for (int i = 0; i < maxLines; i++)
		{
			int moveArea = nextMove (tempBoard, n, oppositeTurn, secondPriority[i]);

			if (moveArea > maxArea)
			{
				maxArea = moveArea;
				maxAreaIndex = i;
			}
		}

		*row = secondPriority[maxAreaIndex][0] - 97;
		*col = secondPriority[maxAreaIndex][1] - 97;
		return 2;
	}
	if (areaHasMoves[2])
	{
		for (int i = 0; i < maxLines; i++)
		{
			int moveArea = nextMove (tempBoard, n, oppositeTurn, thirdPriority[i]);

			if (moveArea > maxArea)
			{
				maxArea = moveArea;
				maxAreaIndex = i;
			}
		}

		*row = thirdPriority[maxAreaIndex][0] - 97;
		*col = thirdPriority[maxAreaIndex][1] - 97;
		return 3;
	}
	if (areaHasMoves[3])
	{
		for (int i = 0; i < maxLines; i++)
		{
			int moveArea = nextMove (tempBoard, n, oppositeTurn, fourthPriority[i]);

			if (moveArea > maxArea)
			{
				maxArea = moveArea;
				maxAreaIndex = i;
			}
		}

		*row = fourthPriority[maxAreaIndex][0] - 97;
		*col = fourthPriority[maxAreaIndex][1] - 97;
		return 4;
	}
	if (areaHasMoves[4])
	{
		for (int i = 0; i < maxLines; i++)
		{
			int moveArea = nextMove (tempBoard, n, oppositeTurn, fifthPriority[i]);

			if (moveArea > maxArea)
			{
				maxArea = moveArea;
				maxAreaIndex = i;
			}
		}

		*row = fifthPriority[maxAreaIndex][0] - 97;
		*col = fifthPriority[maxAreaIndex][1] - 97;
		return 5;
	}

	*row = moves[0][0];
	*col = moves[0][1];

	return 0;
}

int nextMove (const char board[][26], int n, char turn, char move[])
{
	size_t maxLines = 512;
	char (*moves)[3] = calloc (maxLines, sizeof *moves);
	char tempBoard[26][26];
	bool hasMoves = false;

	for (int i = 0; i < 26; i++)
	{
		strcpy (tempBoard[i], board[i]);
	}

	changeBoard (tempBoard, move, turn, n);

	checkPossibleMoves (tempBoard, n, &maxLines, moves, turn, &hasMoves);

	char (*firstPriority)[3] = calloc (maxLines, sizeof *firstPriority);
	char (*secondPriority)[3] = calloc (maxLines, sizeof *secondPriority);
	char (*thirdPriority)[3] = calloc (maxLines, sizeof *thirdPriority);
	char (*fourthPriority)[3] = calloc (maxLines, sizeof *fourthPriority);
	char (*fifthPriority)[3] = calloc (maxLines, sizeof *fifthPriority);

	bool areaHasMoves[5] =
	{ false, false, false, false, false };

	int counters[5] =
	{ 0, 0, 0, 0, 0 };

	char curMove[3];

	for (int i = 0; i < maxLines; i++)
	{
		strcpy (curMove, moves[i]);

		if (isCornerMove (curMove, n))
		{
			strcpy (firstPriority[counters[0]], curMove);
			counters[0]++;
			areaHasMoves[0] = true;
		}

		for (int j = 2; j < n - 2; j++)
		{
			if (curMove[0] - 97 == j)
			{
				if (curMove[1] - 97 == 0 || curMove[1] - 97 == n - 1)
				{
					areaHasMoves[1] = true;
					break;
				}
			}

			if (curMove[0] - 97 == 0 || curMove[0] - 97 == n - 1)
			{
				if (curMove[1] - 97 == j)
				{
					areaHasMoves[1] = true;
					break;
				}
			}
		}

		for (int j = 2; j < n - 2; j++)
		{
			for (int k = 2; k < n - 2; k++)
			{
				if (curMove[0] - 97 == j && curMove[1] - 97 == k)
				{
					areaHasMoves[2] = true;
					break;
				}
			}
		}

		for (int j = 2; j < n - 2; j++)
		{
			if (curMove[0] - 97 == j)
			{
				if (curMove[1] - 97 == 1 || curMove[1] - 97 == n - 2)
				{
					areaHasMoves[3] = true;
					break;
				}
			}

			if (curMove[0] - 97 == 1 || curMove[0] - 97 == n - 2)
			{
				if (curMove[1] - 97 == j)
				{
					areaHasMoves[3] = true;
					break;
				}
			}
		}

		if (curMove[0] - 97 == 0)
		{
			if (curMove[1] - 97 == 1 || curMove[1] - 97 == n - 2)
			{
				if (board[0][0] == turn || board[0][n - 1] == turn)
				{
					areaHasMoves[1] = true;
					break;
				}
				else
				{
					areaHasMoves[4] = true;
					break;
				}
			}
		}

		if (curMove[0] - 97 == 1)
		{
			if (curMove[1] - 97 == 1 || curMove[1] - 97 == n - 2 || curMove[1] - 97 == 0 || curMove[1] - 97 == n - 1)
			{
				if (board[0][0] == turn || board[0][n - 1] == turn)
				{
					areaHasMoves[1] = true;
					break;
				}
				else
				{
					areaHasMoves[4] = true;
					break;
				}
			}
		}

		if (curMove[0] - 97 == n - 1)
		{
			if (curMove[1] - 97 == 1 || curMove[1] - 97 == n - 2)
			{
				if (board[n - 1][0] == turn || board[n - 1][n - 1] == turn)
				{
					areaHasMoves[1] = true;
					break;
				}
				else
				{
					areaHasMoves[4] = true;
					break;
				}
			}
		}

		if (curMove[0] - 97 == n - 2)
		{
			if (curMove[1] - 97 == 1 || curMove[1] - 97 == n - 2 || curMove[1] - 97 == 0 || curMove[1] - 97 == n - 1)
			{
				if (board[n - 1][0] == turn || board[n - 1][n - 1] == turn)
				{
					areaHasMoves[1] = true;
					break;
				}
				else
				{
					areaHasMoves[4] = true;
					break;
				}
			}
		}
	}

	if (areaHasMoves[0])
	{
		return 1;
	}

	if (areaHasMoves[1])
	{
		return 2;
	}
	if (areaHasMoves[2])
	{
		return 3;
	}
	if (areaHasMoves[3])
	{
		return 4;
	}
	if (areaHasMoves[4])
	{
		return 5;
	}

	return 0;
}

bool isCornerMove (char move[3], int n)
{
	if ((move[0] - 97) == 0 || (move[0] - 97) == n - 1)
	{
		if ((move[1] - 97) == 0 || (move[1] - 97) == n - 1)
			return true;
	}

	return false;
}

//int piecesFlipped (char board[][26], char move[], char colour, int n)
//{
//	int row = move[0] - 97; // Row where piece is places
//	int col = move[1] - 97; // Column where piece is placed
//	int piecesFlipped = 0; // The number of pieces that were flipped
//
//	//checks for legality in all directions and then changes the board in the directions that are legal
//	for (int deltaRow = -1; deltaRow <= 1; deltaRow++)
//	{
//		for (int deltaCol = -1; deltaCol <= 1; deltaCol++)
//		{
//			if (checkLegalInDirection (board, n, row, col, colour, deltaRow, deltaCol))
//			{
//				piecesFlipped += piecesFlippedInDirection (board, n, row, col, colour, deltaRow, deltaCol);
//			}
//		}
//	}
//
//	return piecesFlipped;
//}
//
//int piecesFlippedInDirection (char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol)
//{
//	char oppositeColour; // Color opposite of the player
//	int piecesFlipped = 0;
//
//	if (colour == 'W')
//		oppositeColour = 'B';
//	else
//		oppositeColour = 'W';
//
//	// Goes in a given direction and changes the color until it hits a stopping point
//	for (int i = 1; i < n; i++)
//	{
//		int currentRow = row + (i * deltaRow);
//		int currentCol = col + (i * deltaCol);
//
//		if (!positionInBounds (n, currentRow, currentCol))
//			break;
//
//		if (board[currentRow][currentCol] == oppositeColour)
//		{
//			board[currentRow][currentCol] = colour;
//			piecesFlipped++;
//		}
//		else
//			break;
//	}
//
//	return piecesFlipped;
//}

/*
 * Function: checkPossibleMoves
 * -----------------------------
 * Finds all possible legal moves for both players and puts them into a dynamic array
 */
void checkPossibleMoves (char board[][26], int n, size_t *maxLines, char (*moves)[3], char color, bool *hasMoves)
{
	int j = 0;

	// Goes through each position on the board and each direction for every position
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			if (board[row][col] == 'U')
			{
				// If a move is legal in a given direction, it is put into the array of correct moves
				if (checkLegal (board, n, row, col, color))
				{
					// If the array goes over it's allocated size it gets expanded
//					if (j >= (int) *maxLines)
//					{
//						printf("%d\n", j);
//
//						*maxLines *= 2;
//						char (*temp)[3] = realloc (moves, sizeof (*moves) * (*maxLines));
//
//						moves = temp;
//					}

					// Temporary variable representing the current move
					char temp[3] =
					{ (char) 97 + row, (char) 97 + col, '\0' };

					memcpy (moves[j], temp, sizeof *temp * 3);

					*hasMoves = true;
					j++;
				}
			}
		}
	}
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
		if (!positionInBounds (n, currentRow + deltaRow, currentCol + deltaCol))
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
