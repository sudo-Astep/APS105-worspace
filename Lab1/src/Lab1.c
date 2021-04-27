#include <stdio.h>

/*
 * Lab1.c
 *
 *  Created on: Jan 11, 2021
 *
 *  Author: Andrija Stepanovic
 *  Student ID: 1007107239
 *
 *  Takes an input of 3 numbers and outputs half the sum, twice the product and the average of said numbers
 */


int main()
{
	//Variable Declarations
	double inputNumber1; // First inputed number
	double inputNumber2; // Second inputed number
	double inputNumber3; // Third inputed number
	double halfSum; // Half the sum of the inputed numbers
	double twiceProduct; // Twice the product of the inputed numbers
	double avrg; // Average of the inputed numbers

	// Gets input of first number
	printf("Enter First Number: ");
	scanf("%lf", &inputNumber1);

	// Gets input of second number
	printf("Enter Second Number: ");
	scanf("%lf", &inputNumber2);

	//Gets input of third number
	printf("Enter Third Number: ");
	scanf("%lf", &inputNumber3);

	// Calculates half the sum of the inputed numbers
	halfSum = ((inputNumber1 + inputNumber2 + inputNumber3) / 2);

	// Calculates twice the product of the inputed numbers
	twiceProduct = ((inputNumber1 * inputNumber2 * inputNumber3) * 2);

	// Calculates the average of the inputed numbers
	avrg = ((inputNumber1 + inputNumber2 + inputNumber3) / 3);

	// Outputs the required numbers with correct formatting
	printf("Half the Sum: %.2lf\n", halfSum);
	printf("Twice the Product: %.2lf\n", twiceProduct);
	printf("Average: %.2lf\n", avrg);

	return 0;
}


