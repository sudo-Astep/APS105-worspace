#include <stdbool.h>

#include <sys/time.h>
#include <sys/resource.h>

int makeMove (const char board[26][26], int n, char turn, int *row, int *col)
{
	struct rusage usage; // a structure to hold "resource usage" (including time)
	struct timeval start, end; // will hold the start and end times

	getrusage(RUSAGE_SELF, &usage);
	start = usage.ru_utime;
	double timeStart = start.tv_sec + start.tv_usec / 1000000.0; // in seconds


	size_t maxMoves = 128;
	char (*moves)[3] = calloc (maxMoves, sizeof *moves);
	char (*oppMoves)[3] = calloc (maxMoves, sizeof *oppMoves);
	char currTurn;
	char oppTurn;
	int depth;

	bool hasMoves = false;
	bool oppHasTurns = false;

	bool playerMove = true;

	char tempBoard[26][26];

	memcpy (tempBoard, board, 26 * 26);

	if (turn <= 'W')
		currTurn = 'W';
	if (turn <= 'B')
		currTurn = 'B';

	if (turn == 'W')
		oppTurn = 'B';
	else
		oppTurn = 'W';

	if (n % 2 != 0)
	{
		playerMove = false;
		n--;
	}

	checkPossibleMoves (tempBoard, n, &maxMoves, moves, currTurn, &hasMoves);
	checkPossibleMoves (tempBoard, n, &maxMoves, oppMoves, oppTurn, &oppHasTurns);

	if (!hasMoves)
		playerMove = false;
	if (!oppHasTurns)
		playerMove = true;

	depth = currTurn - turn;

	if (isGameOver (hasMoves, oppHasTurns) && findWinner (tempBoard, n) == currTurn)
	{
		return 50;
	}
	else if (isGameOver (hasMoves, oppHasTurns) && findWinner (tempBoard, n) == oppTurn)
	{
		return -50;
	}

	int maxScore = 0;
	int maxScoreIndex = 0;

	if (depth >= 3)
	{
		for (int i = 0; i < maxMoves && strcmp (moves[i], "") != 0; i++)
		{
			int currScore = evaluateMove (board, n, currTurn, oppTurn, moves[i], moves, hasMoves);

			if (currScore > maxScore)
			{
				maxScore = currScore;
			}
		}

		if (playerMove)
			return maxScore;
		else
			return -maxScore;
	}
	if (depth < 3)
	{
		int tempRow = 0;
		int tempCol = 0;

		for (int i = 0; i < maxMoves && strcmp (moves[i], "") != 0; i++)
		{
			if (playerMove)
				n++;

			int currScore = makeMove (tempBoard, n, (oppTurn - (depth + 1)), &tempRow, &tempCol);
			int moveScore = evaluateMove (board, n, currTurn, oppTurn, moves[i], moves, hasMoves);

			if (playerMove)
				currScore += moveScore;
			else
				currScore -= moveScore;

			if (depth == 0 && currScore > maxScore)
			{
				maxScore = currScore;
			}
			else if (currScore < maxScore)
			{
				maxScore = currScore;
				maxScoreIndex = i;
			}

			getrusage(RUSAGE_SELF, &usage);
			end = usage.ru_utime;
			double timeEnd = end.tv_sec +end.tv_usec / 1000000.0; // inseconds
			double totalTime = timeEnd -timeStart;// totalTime now holds the time (in seconds) it takes to run your code

			if (totalTime >= 0.8)
			{
				printf ("Overtime");

				*row = moves[maxScoreIndex][0] - 97;
				*col = moves[maxScoreIndex][1] - 97;

				return maxScore;
			}

			n--;
		}

		*row = moves[maxScoreIndex][0] - 97;
		*col = moves[maxScoreIndex][1] - 97;


		return maxScore;

	}

	return 0;
}

int evaluateMove (const char board[26][26], int n, char turn, char oppTurn, char move[3], char (*moves)[3], bool hasMoves)
{
	size_t maxLines = 128;
	int score = 0;
	bool oppHasTurns = false;

	if (isCornerMove (move, n))
		score += 20;

	for (int i = 0; i < maxLines && strcmp (moves[i], "") != 0; i++)
	{
		if (strcmp (moves[i], "") == 0)
			score = i;
	}

	int interCount;
	int wallCount;

	char tempBoard[26][26];

	memcpy (tempBoard, board, 26 * 26);

	changeBoard (tempBoard, move, turn, n);

	for (int i = 2; i < n - 2; i++)
	{
		for (int j = 2; j < n - 2; j++)
		{
			if (tempBoard[i][j] == turn)
				interCount++;
			else if (tempBoard[i][j] == oppTurn)
				interCount--;
		}

		if (tempBoard[0][i] == turn)
			wallCount++;
		else if (tempBoard[0][i] == oppTurn)
			wallCount--;

		if (tempBoard[n - 1][i] == turn)
			wallCount++;
		else if (tempBoard[n - 1][i] == oppTurn)
			wallCount--;

		if (tempBoard[i][0] == turn)
			wallCount++;
		else if (tempBoard[i][0] == oppTurn)
			wallCount--;

		if (tempBoard[i][n - 1] == turn)
			wallCount++;
		else if (tempBoard[i][n - 1] == oppTurn)
			wallCount--;
	}

	score += interCount;
	score += wallCount;

	return score;

}

//==========================================================================================================================================

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

bool isCornerMove (char move[3], int n)
{
	if ((move[0] - 97) == 0 || (move[0] - 97) == n - 1)
	{
		if ((move[1] - 97) == 0 || (move[1] - 97) == n - 1)
			return true;
	}

	return false;
}

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

bool checkLegalInDirection (char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol)
{
	char oppositeColour; //Color opposite of the current player

	if (colour == 'W')
		oppositeColour = 'B';
	else
		oppositeColour = 'W';

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
