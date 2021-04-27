#include <stdio.h>

/*
 * Lab4Part1.c
 *
 *  Created on: Jan 29, 2021
 *
 *  Author: Andrija Stepanovic
 *  Student ID: 1007107239
 *
 *  Splits a given number of cents into exact change in terms of quarters, dimes, nickels and cents.
 *  Outputs the least possible number of coins for the change
 */

int main()
{
	// Variable Declaration
	int inputValue; // Value inputed by the user
	int numQuarters = 0; // Number of Quarters
	int numDimes = 0; // Number of Dimes
	int numNickels = 0; // Number of Nickels
	int numPennies = 0; // Number of Cents

	// Get's user input
	printf("Please give an amount in cents less than 100: ");
	scanf("%d", &inputValue);

	// Repeats the program until the user inputs an invalid number
	while(inputValue > 0 && inputValue < 100)
	{
		printf("%d cents: ", inputValue);

		// Using integer division to find if there are any quarters in the change
		if(inputValue/25 >= 1)
		{
			// Finds number of quarters
			numQuarters = inputValue/25;
			inputValue -= numQuarters * 25;

			if(numQuarters > 0)
			{
				// Print case for single quarter and multiple quarters
				if(numQuarters == 1)
				{
					printf("1 quarter");
				}
				else
				{
					printf("%d quarters", numQuarters);
				}
			}
		}

		// Using integer division to find if there are any dimes in the change
		if(inputValue/10 >= 1)
		{
			// Finds number of dimes
			numDimes = inputValue/10;
			inputValue -= numDimes * 10;

			if(numDimes > 0)
			{
				// Print case for single dime and multiple dimes
				if(numDimes == 1)
				{
					// Print case for if there are any quarters preceding
					if(numQuarters == 0)
						printf("1 dime");
					else
						printf(", 1 dime");
				}
				else
				{
					// Print case for if there are any quarters preceding
					if(numQuarters == 0)
						printf("%d dimes", numDimes);
					else
						printf(", %d dimes", numDimes);
				}
			}
		}

		// Using integer division to find if there are any nickels in the change
		if(inputValue/5 >= 1)
		{
			// Finds number of nickels
			numNickels = inputValue/5;
			inputValue -= numNickels * 5;

			if(numNickels > 0)
			{
				// Print case for single nickel and multiple nickels
				if(numNickels == 1)
				{
					// Print case for if there are any quarters or dimes preceding
					if(numDimes == 0 && numQuarters == 0)
						printf("1 nickel");
					else
						printf(", 1 nickel");
				}
				else
				{
					// Print case for if there are any quarters or dimes preceding
					if(numDimes == 0 && numQuarters == 0)
						printf("%d nickels", numNickels);
					else
						printf(", %d nickels", numNickels);
				}
			}
		}

		// Pennies is the leftover after all other coin values have been subtracted
		numPennies = inputValue;

		if(numPennies > 0)
		{
			// Print case for single cent and multiple cents
			if(numPennies == 1)
			{
				// Print case for if there are any quarters, dimes or nickels preceding
				if(numDimes == 0 && numQuarters == 0 && numNickels == 0)
					printf("1 cent");
				else
					printf(", 1 cent");
			}
			else
			{
				// Print case for if there are any quarters, dimes or nickels preceding
				if(numDimes == 0 && numQuarters == 0 && numNickels == 0)
					printf("%d cents", numPennies);
				else
					printf(", %d cents", numPennies);
			}
		}

		printf(".\n");

		// Asks for input again
		printf("Please give an amount in cents less than 100: ");
		scanf("%d", &inputValue);

		// Resets values for next run
		numQuarters = 0;
		numDimes = 0;
		numNickels = 0;
		numPennies = 0;
	}

	printf("Goodbye\n");

	return 0;
}
