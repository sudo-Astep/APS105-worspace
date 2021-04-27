#include <stdio.h>

/*
 * Lab4Part2.c
 *
 *  Created on: Jan 29, 2021
 *
 *  Author: Andrija Stepanovic
 *  Student ID: 1007107239
 *
 *  Prints out a triangle with the specified height
 */

int main()
{
	int rows; // The number of rows of the triangle

	// Gets user input
	printf("Enter the number of rows in the triangle: ");
	scanf("%d", &rows);

	// Prints all the rows except the last one
	for(int i = 0; i < rows - 1; i++)
	{
		// Prints first * with appropriate spacing
		printf("%*s", rows - i, "*");

		// Prints the second * with appropriate spacing
		if(i != 0)
			printf("%*s", (2 * i), "*");

		printf("\n");
	}

	// Prints last row with appropriate number of *s
	for(int i = 0; i < (2*rows) - 1; i++)
	{
		printf("*");
	}

	printf("\n");

	return 0;
}
