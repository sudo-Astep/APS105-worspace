#include <stdio.h>
#include <math.h>

/*
 * Lab2Part1.c
 *
 *  Created on: Jan 13, 2021
 *
 *  Author: Andrija Stepanovic
 *  Student ID: 1007107239
 *
 *  Calculates the total price and savings at a store following a promotion
 *  where for every 3 dollars spend the customer saves 0.10 dollars
 */

int main()
{
	double pricePer; // Price per pound of produce
	double weight; // The weight, in pounds, of the produce bought
	double finalPrice; // The price after the savings from the promotion
	double savings; // The savings from the promotion

	double totalPrice; // The total price before the savings


	// User input takes in price per round and weight of produce
	printf("Enter the price per pound: ");
	scanf("%lf", &pricePer);

	printf("Enter the total weight: ");
	scanf("%lf", &weight);

	// Calculates total price
	totalPrice = pricePer * weight;

	// Calculates the savings from the promotion
	savings = ((totalPrice - fmod(totalPrice, 3.0)) / 3.0) * 0.1;

	// Calculates final price
	finalPrice = totalPrice - savings;

	// Output to user
	printf("The total charge is: %.2lf\n", finalPrice);
	printf("You saved: %.2lf\n", savings);

	return 0;
}
