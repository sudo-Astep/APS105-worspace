#include <stdio.h>
#include <math.h>

#define  LAMDA 0.001
#define e 2.718281828

int main() {

	int NumberOfIterations = 3000/150;

    for (int i=1; i<= NumberOfIterations; ++i) {  // Loop for r axis
        printf("--");
    }

    printf("\n");

    for(int t = 0; t <= 150 * NumberOfIterations; t += 150)
    {
    	printf(" |");

    	double power = -1 * LAMDA * t;
    	double r = pow(e, power);

    	int R = r * 50;

    	for(int i = 0; i < R; i++)
    		printf("*");

    	printf("\n");
    }


} //end of main() program

