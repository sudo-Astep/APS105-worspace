#include <stdbool.h>

#include <sys/time.h>
#include <sys/resource.h>

int toNumber (char);
char toChar (int);
bool checkTime (double);
void initializeScoreGrid (int [][26], int);
int positionScore (char[][26], int, int[][26], char);
char opposite (char);
int minimax (const char[][26], int, int, int, int, char, int[][26], bool, double);
void findMove (const char[][26], int, int*, int*, char, int[][26], int, double, bool*);
bool isStable (char[][26], int, int, int);
bool isOver (const char[][26], int);
int winnerCount (char[][26], int, char);
int weightedPositioningScore (char[0][26], int, char);
int positionScore (char[][26], int, int[][26], char);
bool stableChecker (char[][26], int, int, int, int, int);


int makeMove (const char board[26][26], int n, char turn, int *row, int *col)
{
	int potRow = -1;
	int potCol = -1;

	int scoreGrid[26][26];

	initializeScoreGrid (scoreGrid, n);

	bool isLoss = false;

    struct rusage usage; // a structure to hold "resource usage" (including time)
    struct timeval start, end; // will hold the start and end times
    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_utime;
    double timeStart = start.tv_sec + start.tv_usec / 1000000.0; // in seconds

    for (int i = 1; i <= 6; i++)
    {
    	isLoss = false;
    	int tempRow = 0;
    	int tempCol = 0;

    	findMove (board, n, &tempRow, &tempCol, turn, scoreGrid, 6, timeStart, &isLoss);

    	if (checkTime(timeStart))
    	{
    		if (potRow != -1 && potCol != -1)
    		{
    			*row = potRow;
    			*col = potCol;
    		}
    		else
    		{
    			*row = tempRow;
    			*col = tempCol;
    		}
    		return 0;
    	}

    	if (!isLoss && tempRow != -1 && tempCol != -1)
    	{
    		if (checkLegal (board, n, tempRow, tempCol, turn))
    		{
    		potRow = tempRow;
    		potCol = tempCol;
    		}
    	}
    }

    *row = potRow;
    *col = potCol;

    return 0;
}

void findMove (const char board[][26], int n, int *row, int *col, char turn, int scoreGrid[][26], int depth, double timeStart, bool *isLos)
{
	int maxScore = -999;
	int alpha = -999;
	int beta = 999;

	int bRow = -1;
	int bCol = -1;

	char oppTurn = opposite (turn);

	char tempBoard[26][26];

	memcpy (tempBoard, board, sizeof (board[0]) * 26);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (checkLegal (tempBoard, n, i, j, turn))
			{
				char move[3] = {toChar(i), toChar(j), '\0'};
				changeBoard (tempBoard, move, turn, n);

				printf ("%s\n", move);

				int score = minimax (tempBoard, n, depth, alpha, beta, turn, scoreGrid, false, timeStart);

				if (score > maxScore)
				{
					maxScore = score;
					bRow = i;
					bCol = j;

					if (score > alpha)
						alpha = score;

					if (maxScore == 999)
						goto winning;
				}

				if (checkTime (timeStart))
					return;

				memcpy (tempBoard, board, sizeof (board[0]) * 26);
			}
		}
	}

	winning:

	if (maxScore == -999)
		*isLos = true;


	*row = bRow;
	*col = bCol;
}

int minimax (const char board[][26], int n, int depth, int alpha, int beta, char turn, int scoreGrid[][26], bool isPlayer, double timeStart)
{
	int maxScore = -999;
	int minScore = 999;

	char oppTurn = opposite (turn);

	char tempBoard[26][26];

	bool moveAvailable = false;

	memcpy (tempBoard, board, sizeof (board[0]) * 26);

	if (isOver (board, n))
		return winnerCount (tempBoard, n, turn);

	if (depth <= 0)
		return weightedPositioningScore (board, n, turn);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int score;

			if (checkTime (timeStart))
				return -999;

			if (isPlayer)
			{
				if (tempBoard[i][j] == 'U')
				{
				if (checkLegal (tempBoard, n, i, j, turn))
				{
					moveAvailable = true;

					char move[3] = {toChar(i), toChar(j), '\0'};
					changeBoard (tempBoard, move, turn, n);

					score = minimax (tempBoard, n, depth, alpha, beta, turn, scoreGrid, isPlayer, timeStart);

					if (score > maxScore)
						maxScore = score;

					if (maxScore > alpha)
						alpha = maxScore;

					if (alpha >= beta)
						return maxScore;

					memcpy (tempBoard, board, sizeof (board[0])*26);
				}
				}
			}

			else
			{
				if (tempBoard[i][j] == 'U')
				{
				if (checkLegal (tempBoard, n, i, j, oppTurn))
				{
					moveAvailable = true;

					char move[3] = {toChar(i), toChar(j), '\0'};
					changeBoard (tempBoard, move, oppTurn, n);

					score = minimax (tempBoard, n, depth - 1, alpha, beta, oppTurn, scoreGrid, isPlayer, timeStart);

					if (score < minScore)
						minScore = score;

					if (minScore < beta)
						beta = minScore;

					if (alpha >= beta)
						return minScore;

					memcpy (tempBoard, board, sizeof (board[0])*26);
				}
				}
			}
		}
	}

	if (moveAvailable)
	{
		if (isPlayer)
			return maxScore;
		else
			return minScore;
	}
	else
	{
		if (!isPlayer)
			depth--;

		return minimax (board, n, depth, alpha, beta, turn, scoreGrid, !isPlayer, timeStart);
	}

}

int toNumber (char c)
{
	return c - 97;
}

char toChar (int n)
{
	return (n + 97);
}

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

int winnerCount (char board[][26], int n, char turn)
{
	int count = 0;

	char oppTurn = opposite (turn);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (board[i][j] == turn)
				count++;
			if (board[i][j] == oppTurn)
				count--;
		}
	}

	if (count > 0)
		return 999;
	else
		return -999;

	return 0;
}

bool checkTime(double timeStart)
{
    struct rusage usage; // a structure to hold "resource usage" (including time)
    struct timeval start, end; // will hold the start and end times
    getrusage(RUSAGE_SELF, &usage);
    end = usage.ru_utime;
    double timeEnd = end.tv_sec + end.tv_usec / 1000000.0; // in seconds
    double totalTime = timeEnd - timeStart;
    return totalTime>=0.9;
}

void initializeScoreGrid (int scoreGrid[26][26], int n)
{
	scoreGrid[0][0] = 4;
	scoreGrid[0][n-1] = 4;
	scoreGrid[n-1][n-1] = 4;
	scoreGrid[n-1][0] = 4;

	scoreGrid[1][1] = -4;
	scoreGrid[n-2][n-2] = -4;
	scoreGrid[1][n-2] = -4;
	scoreGrid[n-2][1] = -4;

	for (int i = 0; i < 2; i++)
	{
		scoreGrid[1 - i][0 + i] = -3;
		scoreGrid[0 + i][n - 2 + i] = -3;
		scoreGrid[n - 2 + i][0 + i] = -3;
		scoreGrid[n - 1 - i][n - 2 + i] = -3;
	}

	for (int i = 2; i < n - 2; i++)
	{
		scoreGrid[0][i] = 3;
		scoreGrid[i][0] = 3;
		scoreGrid[n - 1][i] = 3;
		scoreGrid[i][n - 1] = 3;

		scoreGrid[1][i] = -1;
		scoreGrid[i][1] = -1;
		scoreGrid[n - 2][i] = -1;
		scoreGrid[i][n - 2] = -1;

		for (int j = 2; j < n - 2; j++)
		{
			scoreGrid[i][j] = 1;
		}
	}
}

int positionScore (char board[][26], int n, int scoreGrid[][26], char turn)
{
	char oppTurn;

	oppTurn = opposite (turn);

	int score = 0;

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

int weightedPositioningScore (char board[0][26], int n, char turn)
{
	double count = 0;
	double oppCount = 0;

	double mobility = 0;
	double oppMobility = 0;

	double corners = 0;
	double oppCorners = 0;

	double stable = 0;
	double oppStable = 0;

	char oppTurn = opposite (turn);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (board[i][j] == turn)
			{
				count++;

				if (isStable (board, n, i, j))
					stable++;
			}

			else if (board[i][j] == oppTurn)
			{
				oppCount++;

				if (isStable (board, n, i, j))
					oppStable++;
			}

			char move[3] = {toChar (i), toChar (j), '\0'};

			if (isCornerMove (move, n))
			{
				if (board[i][j] == turn)
					corners++;

				if (board[i][j] == oppTurn)
					oppCorners++;
			}

			if (board[i][j] == 'U')
			{
				if (checkLegal (board, n, i, j, turn))
					mobility++;

				if (chechLegal (board, n, i, j, oppTurn))
					oppMobility++;
			}

		}
	}

	int totalScore = 0;

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


bool stableChecker (char board[][26], int n, int row, int col, int deltaRow, int deltaCol)
{
	char currPlace = board[row][col];

	char opp = opposite (currPlace);

	int state = 0;
	int reverseState = 0;

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

	if (state == 0)
		return 1;


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

	if (state == 1 && reverseState == 1)
		return 1;

	if (state * reverseState == 2)
		return -1;

	if (state * reverseState == 0)
		return 1;

	return 0;

}


bool isStable (char board[][26], int n, int row, int col)
{
	bool verticalStability;
	bool horizontalStability;
	bool diagonalStability;
	bool diagonalStability2;

	if (board[row][col] != 'U')
	{
		verticalStability = checkStability (board, n, row, col, 1, 0);
		horizontalStability = checkStability (board, n, row, col, 0, 1);
		diagonalStability = checkStability (board, n, row, col, 1, 1);
		diagonalStability2 = checkStability (board, n, row, col, -1, 1);

		if (verticalStability && horizontalStability && diagonalStability && diagonalStability2)
			return true;
	}

	return false;
}


char opposite (char color)
{
	if (color == 'W')
		return 'B';
	else
		return 'W';
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

bool positionInBounds (int n, int row, int col)
{
	if ((row >= n || col >= n) || (row < 0 || col < 0))
		return false;

	return true;
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












