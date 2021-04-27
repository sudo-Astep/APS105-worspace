#include <stdio.h>


int main()
{
	double A = 2;
	double B = 5;
	double C = 2.5;

	printf("%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n", A+B, A*C, A+C, A*C+B, A*(C+B), A*B + B*C + A*C);

	return 0;
}
