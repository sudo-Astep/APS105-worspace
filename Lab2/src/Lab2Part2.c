#include <stdio.h>
#include <math.h>

/*
 * Lab2Part2.c
 *
 *  Created on: Jan 13, 2021
 *
 *  Author: Andrija Stepanovic
 *  Student ID: 1007107239
 *
 *  Rounds the value of a double to the nearest 0.05
 */

int main()
{
	double price; // The price to be rounded

	//Gets input from the user
	printf("Enter the price as a float with two decimals: ");
	scanf("%lf", &price);

	// Rounds the price by multiplying it by 20, which makes .05 = 1, therefore an integer in this case will always translate to a value
	// divisible by 0.5. The rounded value is then divided by 20 to make it the final value rounded to the closest 0.05
	price = round(20 * price) / 20;

	// Output
	printf("The total charge is: %.2lf\n", price);

	return 0;
}
