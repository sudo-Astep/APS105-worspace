#include <stdio.h>
#include <math.h>

/*
 * Lab2Part3.c
 *
 *  Created on: Jan 13, 2021
 *
 *  Author: Andrija Stepanovic
 *  Student ID: 1007107239
 *
 *  Calculates the money in a savings account, A, after t years, with interest r, which is compounded n times per year, given the principal
 *  investment P.
 */

int main()
{
	double principalInvestment; // Principal investment
	double interestRate; // Interest rate
	double compoundingPeriods; // Number of times the investment is compounded per year
	double investmentTime; // The number of years the money is invested for
	double finalValue; // The amount of money in the account after time t has passed

	double base; // The base of the exponential used to calculate A
	double power; // The power of the exponential used to calculate A

	// Input for P
	printf("The amount deposited P: ");
	scanf("%lf", &principalInvestment);

	// Input for r
	printf("The interest rate r: ");
	scanf("%lf", &interestRate);

	// Input for n
	printf("The number of times the interest is compounded n: ");
	scanf("%lf", &compoundingPeriods);

	// Input for t
	printf("The period of time t the money is invested (in years): ");
	scanf("%lf", &investmentTime);


	base = 1 + (interestRate/compoundingPeriods); // Calculates the base of the exponential

	power = compoundingPeriods*investmentTime; // Calculates the power of the exponential

	finalValue = principalInvestment * pow(base, power); // Calculates A

	// Outputs A
	printf("The future value of the investment A: %.2lf\n", finalValue);

}
