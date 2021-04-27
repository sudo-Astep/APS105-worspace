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

#include <sys/time.h>
#include <sys/resource.h>

// =========   Function declaration  ==========

bool isLegalMove (char[], char[][3], int);
void changeBoard (char[][26], char[], char, int);
void changeBoardInDirection (char[][26], int, int, int, char, int, int);
void checkPossibleMoves (char[][26], int, size_t*, char (*moves)[3], char, bool*);
bool isGameOver (bool, bool);
int piecesFlipped (char[][26], char[], char, int);
int piecesFlippedInDirection (char[][26], int, int, int, char, int, int);
void resetMoves (char (*whiteMoves)[3], char (*blackMoves)[3], bool*, bool*, size_t, size_t);
char findWinner (char[][26], int);
bool isCornerMove (char[], int);
bool checkLegal (char[][26], int, int, int, char);
int toNumber (char);
char toChar (int);
bool checkTime (double);
void initializeScoreGrid (int [][26], int);
double positionScore (char[][26], int, int[][26], char);
char opposite (char);
double minimax (const char[][26], int, int, double, double, char, int[][26], bool, double);
void findMove (const char[][26], int, int*, int*, char, int[][26], int, double);
bool isStable (char[][26], int, int, int);
bool isOver (const char[][26], int);
double winnerCount (char[][26], int, char);
double weightedPositioningScore (char[0][26], int, char);
double positionScore (char[][26], int, int[][26], char);
bool stableChecker (char[][26], int, int, int, int, int);

#ifndef COMPETE_MODE // DO NOT DELETE THIS LINE
int main (void)
{
	// Variable Declarations
	int n; // Board size
	size_t maxLinesWhite = 256; // Variable that dictates the size of the whiteMoves dynamic array, so it doesn't have to be reallocated constantly (as in every time something is added)
	size_t maxLinesBlack = 256; // variable that dictates the size of the blackMoves dynamic array
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
	board[(int) ((n / 2.0) - 0.5)][(int) ((n / 2.0) - 0.5)] = 'B';
	board[(int) ((n / 2.0) + 0.5)][(int) ((n / 2.0) + 0.5)] = 'B';
	board[(int) ((n / 2.0) + 0.5)][(int) ((n / 2.0) - 0.5)] = 'W';
	board[(int) ((n / 2.0) - 0.5)][(int) ((n / 2.0) + 0.5)] = 'W';

	printf ("Computer plays (B/W) : ");
	scanf (" %c", &computerColor);

	printBoard (board, n);

	checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
	checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);

	int row = 0;
	int col = 0;

	while (!isGameOver (whiteHasMoves, blackHasMoves))
	{
		char playerMove[3];

		if (computerColor == 'B')
		{
			if (blackHasMoves)
			{
				makeMove (board, n, computerColor, &row, &col);

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
				printf ("Enter move for colour W (RowCol): ");
				scanf ("%s", playerMove);

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
				printf ("Enter move for colour B (RowCol): ");
				scanf ("%s", playerMove);

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
				makeMove (board, n, computerColor, &row, &col);

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
			printf ("%c\n", board[i][j]);

			if (board[i][j] == 'W')
				whiteScore++;

			if (board[i][j] == 'B')
				blackScore++;
		}
	}

	printf ("%d %d\n", whiteScore, blackScore);

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
	int potRow = -1;
	int potCol = -1; // Potential row and column

	int scoreGrid[26][26]; // Static grid that evaluates each position on the board

	initializeScoreGrid (scoreGrid, n); // Initializes the static score grid

    struct rusage usage; // a structure to hold "resource usage" (including time)
    struct timeval start, end; // will hold the start and end times
    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_utime;
    double timeStart = start.tv_sec + start.tv_usec / 1000000.0; // in seconds

    // Goes through different depths, makes sure it doesn't go overtime searching too deep in a single branch
    for (int i = 1; i <= 6; i++)
    {
    	int tempRow = 0;
    	int tempCol = 0; // temporary row and column sent to findMove

    	// Finds the best move it can from the current board state
    	findMove (board, n, &tempRow, &tempCol, turn, scoreGrid, i, timeStart);

    	// Checks that the algorithm doesn't go overtime
    	if (checkTime(timeStart))
    	{
    		break;
    	}

    	// Replaces the potential move with new one found with by going deeper
    	// Runs on the assumption that going deeper necessarily means the chosen move is better
    	if (tempRow != -1 && tempCol != -1)
    	{
				potRow = tempRow;
				potCol = tempCol;
    	}
    }

    // Sets row and column to values found
    *row = potRow;
    *col = potCol;

    return 0;
}

/*
 * Function: findMove
 * --------------------
 * Finds the best possible move given the current board state. Runs using the minimax algorithm with alpha-beta pruning
 */
void findMove (const char board[][26], int n, int *row, int *col, char turn, int scoreGrid[][26], int depth, double timeStart)
{
	double maxScore = -99999; // Maximum score found amongst the moves
	double alpha = -99999; // Alpha value
	double beta = 99999; // Beta value

	int bestRow = -1;
	int bestCol = -1; // Best row and column found

	char tempBoard[26][26]; // Temporary board that can be changed if needed

	// copies current board state into temporary board
	memcpy (tempBoard, board, sizeof (board[0]) * 26);

	// Goes through each board position and checks possible moves
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (checkLegal (tempBoard, n, i, j, turn))
			{
				// Changes board to reflect move made
				char move[3] = {toChar(i), toChar(j), '\0'};
				changeBoard (tempBoard, move, turn, n);

				// Runs minimax to evaluate the move
				double score = minimax (tempBoard, n, depth, alpha, beta, turn, scoreGrid, false, timeStart);

				// Changes best row and col if better move is found
				if (score >= maxScore)
				{
					maxScore = score;
					bestRow = i;
					bestCol = j;

					if (score > alpha)
						alpha = score;

					// If move leads to a winning branch it returns that move
					if (maxScore == 99999)
						goto winning;
				}

				// Checks for time constraint
				if (checkTime (timeStart))
					goto winning;

				// Restores board state
				memcpy (tempBoard, board, sizeof (board[0]) * 26);
			}
		}
	}

	winning:

	*row = bestRow;
	*col = bestCol;
}

/*
 * Function: minimax
 * ----------------------
 * Uses minimax algorithm to check for best possible move
 */
double minimax (const char board[][26], int n, int depth, double alpha, double beta, char turn, int scoreGrid[][26], bool isPlayer, double timeStart)
{
	double maxScore = -99999; // maximum score found
	double minScore = 99999; // minimum score found

	char oppTurn = opposite (turn); // Opponent color

	char tempBoard[26][26]; // Temporary board

	bool moveAvailable = false;

	// Copies board state
	memcpy (tempBoard, board, sizeof (board[0]) * 26);

	// Handles if game is won or lost
	if (isOver (board, n))
		return winnerCount (tempBoard, n, turn);

	// If max depth is reached returns score
	if (depth <= 0)
		return weightedPositioningScore (board, n, turn);

	// Checks all possible moves
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			double score;

			// Checks for overtime
			if (checkTime (timeStart))
				return -99999;

			// If current move is player's move
			if (isPlayer)
			{
				// checks move legality
				if (checkLegal (tempBoard, n, i, j, turn))
				{
					moveAvailable = true;

					// Changes board state to reflect move
					char move[3] = {toChar(i), toChar(j), '\0'};
					changeBoard (tempBoard, move, turn, n);

					// Re-runs minimax to evaluate next move
					score = minimax (tempBoard, n, depth, alpha, beta, turn, scoreGrid, !isPlayer, timeStart);

					// If new maximum score is found
					if (score > maxScore)
						maxScore = score;

					if (maxScore > alpha)
						alpha = maxScore;

					if (alpha >= beta)
						return maxScore;

					// Restores board state
					memcpy (tempBoard, board, sizeof (board[0])*26);
				}
			}

			// Handles opponent turn
			else
			{
				// Checks move legality
				if (checkLegal (tempBoard, n, i, j, oppTurn))
				{
					moveAvailable = true;

					// Changes board to satte to reflect move
					char move[3] = {toChar(i), toChar(j), '\0'};
					changeBoard (tempBoard, move, oppTurn, n);

					// Reruns minimax to evaluate move
					score = minimax (tempBoard, n, depth - 1, alpha, beta, turn, scoreGrid, !isPlayer, timeStart);

					// If new minimum score is found
					if (score < minScore)
						minScore = score;

					if (minScore < beta)
						beta = minScore;

					if (alpha >= beta)
						return minScore;

					// Restores board state
					memcpy (tempBoard, board, sizeof (board[0])*26);
				}
			}
		}
	}


	// returns max score if it's player move, min score if it's opponents move
	if (moveAvailable)
	{
		if (isPlayer)
			return maxScore;
		else
			return minScore;
	}
	// If there was no move available runs minimax for next player
	else
	{
		if (!isPlayer)
			depth--;

		return minimax (board, n, depth, alpha, beta, turn, scoreGrid, !isPlayer, timeStart);
	}

}

/*
 * Function: toNumber
 * -------------------
 * Converts character coordinate to number coordinate
 */
int toNumber (char c)
{
	return c - 97;
}

/*
 * Function: toChar
 * -------------------
 * Converts number coordinate to character coordinate
 */
char toChar (int n)
{
	return n + 97;
}

/*
 * Function: isOver
 * -------------------
 * Checks if game is over
 */
bool isOver (const char board[][26], int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (checkLegal (board, n, i, j, 'W') || checkLegal (board, n, i, j, 'B'))
				return false;
		}
	}

	return true;
}

/*
 * Function: winnerCount
 * ----------------------
 * Finds the winner and returns appropriate score
 */
double winnerCount (char board[][26], int n, char turn)
{
	int count = 0;

	char oppTurn = opposite (turn);

	// Checks each possition
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			// Adds 1 if player has the position, -1 is opponent has Position
			if (board[i][j] == turn)
				count++;
			if (board[i][j] == oppTurn)
				count--;
		}
	}

	// Returns if player or opponent wins
	if (count > 0)
		return 99999;
	else
		return -99999;

	return 0;
}

/*
 * Function: checkTime
 * --------------------
 * Checks if program is overtime
 */
bool checkTime(double timeStart)
{
    struct rusage usage; // a structure to hold "resource usage" (including time)
    struct timeval start, end; // will hold the start and end times
    getrusage(RUSAGE_SELF, &usage);
    end = usage.ru_utime;
    double timeEnd = end.tv_sec + end.tv_usec / 1000000.0; // in seconds
    double totalTime = timeEnd - timeStart;


    return totalTime >= 0.8;
}

/*
 * Function: initializeScoreGrid
 * -------------------------------
 * Initializes the static position scoring grid
 */
void initializeScoreGrid (int scoreGrid[26][26], int n)
{
	// Corners are best
	scoreGrid[0][0] = 4;
	scoreGrid[0][n-1] = 4;
	scoreGrid[n-1][n-1] = 4;
	scoreGrid[n-1][0] = 4;

	// Diagonal corner buffers a bad :(
	scoreGrid[1][1] = -4;
	scoreGrid[n-2][n-2] = -4;
	scoreGrid[1][n-2] = -4;
	scoreGrid[n-2][1] = -4;

	// Corner buffers are also bad
	for (int i = 0; i < 2; i++)
	{
		scoreGrid[1 - i][0 + i] = -3;
		scoreGrid[0 + i][n - 2 + i] = -3;
		scoreGrid[n - 2 + i][0 + i] = -3;
		scoreGrid[n - 1 - i][n - 2 + i] = -3;
	}

	for (int i = 2; i < n - 2; i++)
	{
		// Walls are good :)
		scoreGrid[0][i] = 3;
		scoreGrid[i][0] = 3;
		scoreGrid[n - 1][i] = 3;
		scoreGrid[i][n - 1] = 3;

		// Wall buffers are not ideal
		scoreGrid[1][i] = -1;
		scoreGrid[i][1] = -1;
		scoreGrid[n - 2][i] = -1;
		scoreGrid[i][n - 2] = -1;

		// Everything else is alright
		for (int j = 2; j < n - 2; j++)
		{
			scoreGrid[i][j] = 1;
		}
	}
}

/*
 * Function: positionScore
 * --------------------------
 * Finds the positioning score of each player based on static score grid
 */
double positionScore (char board[][26], int n, int scoreGrid[][26], char turn)
{
	char oppTurn; // Opponents color

	oppTurn = opposite (turn);

	double score = 0;

	// Counts the score of each spot
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (board[i][j] == turn)
				score += scoreGrid[i][j];
			if (board[i][j] == oppTurn)
				score -= scoreGrid[i][j];
		}
	}

	return score;

}

/*
 * Function: weightedPositioningScore
 * -------------------------------------
 * Finds the weighted positioning score of the function based on:
 *
 * 		Mobility
 * 		Corners occupied
 * 		Stability
 * 		Number of pieces on the board
 */
double weightedPositioningScore (char board[0][26], int n, char turn)
{
	double count = 0;
	double oppCount = 0; // Number of pieces on the board

	double mobility = 0;
	double oppMobility = 0; // Moves available

	double corners = 0;
	double oppCorners = 0; // Coreners occupied

	double stable = 0;
	double oppStable = 0; // Stable positions occupied

	char oppTurn = opposite (turn); // Opponent turn

	// Checks each position on the board
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			// Checks number of pieces on the board
			if (board[i][j] == turn)
			{
				count++;

				// Checks stability
				if (isStable (board, n, i, j))
					stable++;
			}

			else if (board[i][j] == oppTurn)
			{
				oppCount++;

				if (isStable (board, n, i, j))
					oppStable++;
			}

			// Re-runs minimax to evaluate next move
			char move[3] = {toChar (i), toChar (j), '\0'};

			// Checks corner moves
			if (isCornerMove (move, n))
			{
				if (board[i][j] == turn)
					corners++;

				if (board[i][j] == oppTurn)
					oppCorners++;
			}

			// Checks mobility
			if (board[i][j] == 'U')
			{
				if (checkLegal (board, n, i, j, turn))
					mobility++;

				if (checkLegal (board, n, i, j, oppTurn))
					oppMobility++;
			}

		}
	}

	double totalScore = 0;

	// Weighted for each scoring category
	if (count + oppCount != 0)
		totalScore += 20 * ((count - oppCount) / (count + oppCount));

	if (stable + oppStable != 0)
		totalScore += 33 * ((stable - oppStable) / (stable + oppStable));

	if (corners + oppCorners != 0)
		totalScore += 50 * ((corners - oppCorners) / (corners + oppCorners));

	if (mobility + oppMobility != 0)
		totalScore += 100 * ((mobility - oppMobility) / (mobility + oppMobility));



	return totalScore;

}

/*
 * Function: stableChecker
 * ----------------------------
 * Checks piece stability in a given direction
 * 1 = stable, 0 = unknown, -1 = unstable
 */
bool stableChecker (char board[][26], int n, int row, int col, int deltaRow, int deltaCol)
{
	char currPlace = board[row][col]; // Current position piece

	char opp = opposite (currPlace); // Opposite of current piece

	int state = 0; // State 1 = opposite color, 0 = current color, 2 = empty
	int reverseState = 0; // Same, but in opposite direction

	// Checks stability in one direction
	for (int i = 1; positionInBounds (n, row + (deltaRow*i), col + (deltaCol*i)); i++)
	{
		if (board [row + (deltaRow*i)][col + (deltaCol*i)] == opp)
		{
			state = 1;
			break;
		}
		else if (board [row + (deltaRow*i)][col + (deltaCol*i)] == 'U')
		{
			state = 2;
			break;
		}
	}

	// Implies stability
	if (state == 0)
		return 1;

	// Checks stability in opposite direction
	for (int i = -1; positionInBounds (n, row + (deltaRow*i), col + (deltaCol*i)); i--)
	{
		if (board [row + (deltaRow*i)][col + (deltaCol*i)] == opp)
		{
			reverseState = 1;
			break;
		}
		else if (board [row + (deltaRow*i)][col + (deltaCol*i)] == 'U')
		{
			reverseState = 2;
			break;
		}
	}

	// Stability between 2 pieces of opposite color
	if (state == 1 && reverseState == 1)
		return 1;

	// If either side has an empty spot and other has opposite color, unstable
	if (state * reverseState == 2)
		return -1;

	// If either side has current color, it's stable is this direction
	if (state * reverseState == 0)
		return 1;

	return 0;

}

/*
 * Function: isStable
 * -----------------------
 * checks stability for a given board position
 */
bool isStable (char board[][26], int n, int row, int col)
{
	bool verticalStability;
	bool horizontalStability;
	bool diagonalStability;
	bool diagonalStability2;

	// If the current position isn't empty
	if (board[row][col] != 'U')
	{
		// All the possible directions bust have stability for position to be stable
		verticalStability = stableChecker (board, n, row, col, 1, 0);
		horizontalStability = stableChecker (board, n, row, col, 0, 1);
		diagonalStability = stableChecker (board, n, row, col, 1, 1);
		diagonalStability2 = stableChecker (board, n, row, col, -1, 1);

		if (verticalStability && horizontalStability && diagonalStability && diagonalStability2)
			return true;
	}

	return false;
}

/*
 * Function: opposite
 * ---------------------
 * Returns the collor of the opposing player
 */
char opposite (char color)
{
	if (color == 'W')
		return 'B';
	else
		return 'W';
}

/*
 * Function: isCornerMove
 * --------------------------
 * checks if a move is into the corner
 */
bool isCornerMove (char move[3], int n)
{
	if ((move[0] - 97) == 0 || (move[0] - 97) == n - 1)
	{
		if ((move[1] - 97) == 0 || (move[1] - 97) == n - 1)
			return true;
	}

	return false;
}

/*
 * Function: piecesFlipped
 * -------------------------
 * Counts the number of pieces a move flips
 */
int piecesFlipped (char board[][26], char move[], char colour, int n)
{
	int row = move[0] - 97; // Row where piece is places
	int col = move[1] - 97; // Column where piece is placed
	int piecesFlipped = 0; // The number of pieces that were flipped

	//checks for legality in all directions and then changes the board in the directions that are legal
	for (int deltaRow = -1; deltaRow <= 1; deltaRow++)
	{
		for (int deltaCol = -1; deltaCol <= 1; deltaCol++)
		{
			if (checkLegalInDirection (board, n, row, col, colour, deltaRow, deltaCol))
			{
				piecesFlipped += piecesFlippedInDirection (board, n, row, col, colour, deltaRow, deltaCol);
			}
		}
	}

	return piecesFlipped;
}

/*
 * Function: piecesFlippedIndirection
 * -------------------------
 * Counts the number of pieces a move flips in a given direction
 */
int piecesFlippedInDirection (char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol)
{
	char oppositeColour; // Color opposite of the player
	int piecesFlipped = 0;

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
		{
			board[currentRow][currentCol] = colour;
			piecesFlipped++;
		}
		else
			break;
	}

	return piecesFlipped;
}

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
					if (j >= *maxLines - 1)
					{
						*maxLines *= 2;
						char (*temp)[3] = realloc (moves, sizeof *moves * *maxLines);
						moves = temp;
					}

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
	if (board[row][col] != 'U')
		return false;

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
