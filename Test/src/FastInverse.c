#include <stdio.h>
#include <time.h>
#include <math.h>

float fastInverse(float number);

/*
 * FastInverse.c
 *
 *  Created on: Feb 12, 2021
 *      Author: ace
 */

int main()
{
	float number;
	float temp;
	clock_t start, end;
	double tCPU;

	scanf("%f", &number);

	start = clock();
	for(int i = 0; i < 10000; i++)
		temp = fastInverse(number);
	end = clock();

	tCPU = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("%lf\n", tCPU);

	start = clock();
	for(int i = 0; i < 10000; i++)
		temp = 1/sqrt(number);
	end = clock();

	tCPU = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("%lf\n", tCPU);
}


float fastInverse(float number)
{
	int i;
	float x2, y;
	const float threeHalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = * (int *) &y;
	i = 0x5f3759df - (i >> 1);
	y = * (float *) &i;
	y = y * (threeHalfs - (x2 * y * y));
//	y = y * (threeHalfs - (x2 * y * y));

	return y;
}
