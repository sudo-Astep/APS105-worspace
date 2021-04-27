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

typedef struct move
{
	char move[3];
	int score;
	struct move *next;
} node;

// =========   Function declaration  ==========

bool isLegalMove (char[], char[][3], int);
void changeBoard (char[][26], char[], char, int);
void changeBoardInDirection (char[][26], int, int, int, char, int, int);
void checkPossibleMoves (char[][26], int, size_t*, char (*moves)[3], char, bool*);
bool isGameOver (bool, bool);
int piecesFlipped (char[][26], char[], char, int);
int piecesFlippedInDirection (char[][26], int, int, int, char, int, int);
void resetMoves (char (*whiteMoves)[3], char (*blackMoves)[3], bool*, bool*);
void insertFirst (char[], int, node**);
void reverse (node**);
char findWinner (char[][26], int);
bool checkLegal (char[][26], int, int, int, char);

#ifndef COMPETE_MODE // DO NOT DELETE THIS LINE
int main (void)
{
	// Variable Declarations
	int n; // Board size
	size_t maxLinesWhite = 10; // Variable that dictates the size of the whiteMoves dynamic array, so it doesn't have to be reallocated constantly (as in every time something is added)
	size_t maxLinesBlack = 10; // variable that dictates the size of the blackMoves dynamic array
	char (*whiteMoves)[3] = calloc (maxLinesWhite, sizeof *whiteMoves); // Array that keeps all the possible moves of the white player
	char (*blackMoves)[3] = calloc (maxLinesBlack, sizeof *blackMoves); // Array that keeps all the possible moves of the black player
	char board[26][26]; // Board that the game is played on
	char computerColor; // Computer piece color
	bool whiteHasMoves = false;
	bool blackHasMoves = false;

	resetMoves (whiteMoves, blackMoves, &whiteHasMoves, &blackHasMoves);

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

	// Gets the color that the computer will be playing
	printf ("Computer plays (B/W) : ");
	scanf (" %c", &computerColor);

	// Prints board
	printBoard (board, n);

	checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
	checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);

	int row = 0;
	int col = 0;

	// Game playing loop
	while (!isGameOver (whiteHasMoves, blackHasMoves))
	{
		char playerMove[3];

		if (computerColor == 'B')
		{
			// Plays computer move if the computer is black
			if (blackHasMoves)
			{
				// Makes computer move
				makeMove (board, n, computerColor, &row, &col);

				// Puts computer move into a string, so it can change the board
				char computerMove[3] =
				{ 97 + row, 97 + col, '\0' };

				printf ("Computer places %c at %s.\n", computerColor, computerMove);

				// Makes the computer move on the board
				changeBoard (board, computerMove, computerColor, n);

				// Prints board
				printBoard (board, n);

			}
			// Handles the case where computer has no moves
			else
			{
				printf ("B player has no available moves\n");
			}

			// Resets moves
			resetMoves (whiteMoves, blackMoves, &whiteHasMoves, &blackHasMoves);

			// Finds moves for the new board state
			checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
			checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);

			// Handles player move
			if (whiteHasMoves)
			{
				// Player input
				printf ("Enter move for colour W (RowCol): ");
				scanf ("%s", playerMove);

				// Checks for move legality
				if (isLegalMove (playerMove, whiteMoves, maxLinesWhite))
				{
					changeBoard (board, playerMove, 'W', n);

					printBoard (board, n);
				}
				// game over if player inputs invalid move
				else
				{
					printf ("Invalid move.\n");
					printf ("B player wins.");
					return 0;
				}
			}
			// Handles if white has no moves
			else if (!isGameOver (whiteHasMoves, blackHasMoves))
			{
				printf ("W player has no valid move.\n");
			}

			// Resets moves arrays
			resetMoves (whiteMoves, blackMoves, &whiteHasMoves, &blackHasMoves);

			// Finds possible moves for current board state
			checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
			checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);
		}

		// Handles when computer is white
		if (computerColor == 'W')
		{
			if (blackHasMoves)
			{
				// Gets player input
				printf ("Enter move for colour B (RowCol): ");
				scanf ("%s", playerMove);

				// Checks move legality
				if (isLegalMove (playerMove, blackMoves, maxLinesBlack))
				{
					// Modifies board if move is legal
					changeBoard (board, playerMove, 'B', n);
					printBoard (board, n);

				}
				// Game ends if move is illegal
				else
				{
					printf ("Invalid move.\n");
					printf ("W player wins.\n");
					return 0;
				}
			}
			// Handles if black has no moves left
			else
			{
				printf ("B player has no valid move.\n");
			}

			// resets moves
			resetMoves (whiteMoves, blackMoves, &whiteHasMoves, &blackHasMoves);

			// Chekcs possible moves for new board state
			checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
			checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);

			// Handles computer move
			if (whiteHasMoves)
			{
				// Makes move
				makeMove (board, n, computerColor, &row, &col);

				char computerMove[3] =
				{ 97 + row, 97 + col, '\0' };

				printf ("Computer places %c at %s.\n", computerColor, computerMove);

				// Modifies board
				changeBoard (board, computerMove, computerColor, n);

				printBoard (board, n);
			}
			// Handles when computer has no moves to make
			else if (!isGameOver (whiteHasMoves, blackHasMoves))
			{
				printf ("W player has no valid move.\n");
			}

			// resets moves
			resetMoves (whiteMoves, blackMoves, &whiteHasMoves, &blackHasMoves);

			// Finds possible moves for current board state
			checkPossibleMoves (board, n, &maxLinesWhite, whiteMoves, 'W', &whiteHasMoves);
			checkPossibleMoves (board, n, &maxLinesBlack, blackMoves, 'B', &blackHasMoves);
		}
	}

	char winner;

	// Finds and prints the winner of the game
	winner = findWinner (board, n);

	printf ("%c player wins.", winner);

	return 0;
}
#endif // DO NOT DELETE THIS LINE

// ========= Function implementation ==========

/*
 * Function: findWinner
 * -----------------------
 * Finds which player won the game by counting the number of each players
 * pieces on the board. Returns the winner's color.
 */
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

/*
 * Function: isGameOver
 * --------------------------
 * Checks if the game is over
 */
bool isGameOver (bool whiteHasMoves, bool blackHasMoves)
{
	if (!(whiteHasMoves || blackHasMoves))
	{
		return true;
	}

	return false;
}

/*
 * Function: resetMoves
 * -----------------------
 * Resets the moves arrays to a default state
 */
void resetMoves (char (*whiteMoves)[3], char (*blackMoves)[3], bool *whiteHasMoves, bool *blackHasMoves)
{
	// Default move
	char nullMove[3] =
	{ '0', '0', '\0' };

	// Makes arrays to replace old ones
	char (*newWhiteMoves)[3] = calloc ((size_t) 10, sizeof *whiteMoves); // Array that keeps all the possible moves of the white player
	char (*newBlackMoves)[3] = calloc ((size_t) 10, sizeof *blackMoves); // Array that keeps all the possible moves of the black player

	strcpy (newWhiteMoves[0], nullMove);
	strcpy (newBlackMoves[0], nullMove);

	// Resets arrays to default arrays
	blackMoves = newBlackMoves;
	whiteMoves = newWhiteMoves;

	// Resets booleans
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

/*
 * Function:  makeMove
 * --------------------
 * The player "turn" makes a move at (row, col).
 * Note: This function MUST NOT change board, only row and col can be changed to reflect the move.
 */
int makeMove (const char board[26][26], int n, char turn, int *row, int *col)
{
	char tempBoard[26][26]; // temporary board so that board doesn't have to be modified
	size_t maxLines = 10;
	char (*moves)[3] = calloc (maxLines, sizeof *moves); // Array that holds possible moves
	char finalMove[3]; // Move that will be returned
	struct move *head = NULL; // Head of linked list
	struct move *current = NULL; // Current element in linked list
	struct move *max = NULL; // Element that holds maximum score
	int maxScore = 0; // Maximum score
	bool hasMoves = false;

	// copies board to temporary board
	memcpy (tempBoard, board, board[0] * 26);

	// Finds possible moves for the computer
	checkPossibleMoves (tempBoard, n, &maxLines, moves, turn, &hasMoves);

	// Number of pieces flipped
	int flipped = 0;

	// finds the score for each move and adds them to the linked list
	for (int i = 0; i <= maxLines; i++)
	{
		flipped = piecesFlipped (tempBoard, moves[i], turn, n);
		insertFirst (moves[i], flipped, &head);
	}

	// reverses linked list
	reverse (&head);

	current = head;

	// Goes through the linked list and finds the element that has the highest score
	for (int i = 0; current->next != NULL; i++)
	{
		if (current->score > maxScore)
		{
			maxScore = current->score;

			max = current;
		}

		current = current->next;
	}

	// Sets finalMove
	strcpy (finalMove, max->move);

	// sets the final row and col
	*row = finalMove[0] - 97;
	*col = finalMove[1] - 97;

	// No reason for this anymore, but it doesn't make a difference
	if (hasMoves)
		return 1;

	return 0;
}


/*
 * Function: piecesFlipped
 * -------------------------
 * Counts the cumber of pieces flipped in a given move
 */
int piecesFlipped (char board[][26], char move[], char colour, int n)
{
	int row = move[0] - 97; // Row where piece is places
	int col = move[1] - 97; // Column where piece is placed
	int piecesFlipped = 0; // The number of pieces that were flipped

	//checks for legality in all directions and then changes the board in the directions that are legal
	//and counts the number of pieces that got flipped
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

int piecesFlippedInDirection (char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol)
{
	char oppositeColour; // Color opposite of the player
	int piecesFlipped = 0;

	if (colour == 'W')
		oppositeColour = 'B';
	else
		oppositeColour = 'W';

	// Goes in a given direction and finds the number of pieces that would be flipped
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
 * Function: checkLegal
 * ------------------------
 * Checks for move legality
 */
bool checkLegal (char board[][26], int n, int row, int col, char color)
{
	// Goes through all possible directions and checks if any of them have a legal move
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
				for (int deltaRow = -1; deltaRow <= 1; deltaRow++)
				{
					for (int deltaCol = -1; deltaCol <= 1; deltaCol++)
					{
						// If a move is legal in a given direction, it is put into the array of correct moves
						if (checkLegal (board, n, row, col, color))
						{
							// If the array goes over it's allocated size it gets expanded
							if (j >= *maxLines)
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
	}
}

/*
 * Function: insertFirst
 * ------------------------
 * Inserts an element at the beginning of the linked list
 */
void insertFirst (char move[3], int score, node **head)
{
	node *link = (node*) malloc (sizeof(node));

	strcpy (link->move, move);
	link->score = score;
	link->next = *head;

	*head = link;
}

/*
 * Function: reverse
 * ---------------------
 * Reverses the elements in a linked list
 */
void reverse (node **head)
{
	node *previous = NULL;
	node *current = *head;
	node *next;

	while (current != NULL)
	{
		next = current->next;
		current->next = previous;
		previous = current;
		current = next;
	}

	*head = previous;
}

