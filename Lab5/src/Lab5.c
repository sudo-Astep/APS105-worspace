#include <stdio.h>
#include "Lab5.h"

/*
 * Lab5.c
 *
 *  Created on: Feb 5, 2021
 *
 *  Author: Andrija Stepanovic
 *  Student ID: 1007107239
 *
 *  Prints out a specified number of rows of the Pascal's triangle
 */

int main ()
{
	int rows = 0; // The number of rows of the triangle

	// repeats prompting the user until they input an invalid number
	while (rows >= 0 && rows <= 13)
	{
		// Gets user input
		printf ("Enter the number of rows in the triangle: ");
		scanf ("%d", &rows);

		// Runs triangle function if the user input is valid
		if (rows > 0 && rows <= 13)
			triangle (rows);
	}

	return 0;
}


/*
 * Function: triangle
 * ------------------
 * Prints Pascal's triangle with the passed number of rows
 *
 * rows: number of rows of Pascal's triangle to print
 */
void triangle (int rows)
{
	// Loop that goes through each row of the triangle
	for (int n = 0; n < rows; n++)
	{
		// Spaces the row
		space (n, rows);

		// Loop that goes through every value in the given row
		for (int r = n; r >= 0; r--)
		{
			// Calculates the value of nCr and spaces it
			int C = choose (n, r);

			printf ("%-3d", C);

			if (r != 0)
				printf ("   ");
		}

		printf ("\n");
	}
}


/*
 * Function: choose
 * ----------------
 * Computes the value of a number in Pascal's triangle
 * with position (n, n-r)
 *
 * n: The row of the current value in the triangle
 * r: The value used to find the position of the value
 * 		in the row, using (n - r)
 *
 * returns: The value of the number in the above spot
 * 			on the triangle
 */
int choose (int n, int r)
{
	int nFactorial; // n factorial
	int rFactorial; // r factorial
	int nRFactorial; // Factorial of n - r
	int finalValue; // Final value of the given triangle position

	nFactorial = factorial (n);
	rFactorial = factorial (r);
	nRFactorial = factorial (n - r);

	finalValue = nFactorial / (rFactorial * nRFactorial);

	return finalValue;
}


/*
 * Function: factorial
 * -------------------
 * Recursively calculates the factorial of passed value
 *
 * n: The value to calculate the factorial of
 *
 * returns: The factorial of n
 */
int factorial (int n)
{
	if (n == 0)
		return 1;
	else
	{
		n = n * factorial (n - 1);
		return n;
	}
}


/*
 * Function: space
 * ---------------
 * Calcualtes and prints the correct number of spaces for the given row
 *
 * currentRow: The current row of the triangle
 * totalRows: The total number of rows in the triangle
 */
void space (int currentRow, int totalRows)
{
	int spaces; // The number of spaces to print

	spaces = 3 * (totalRows - currentRow - 1);

	for (int i = 0; i < spaces; i++)
		printf (" ");
}

