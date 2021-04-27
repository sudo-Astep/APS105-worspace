#include <stdio.h>

/* Q1:

Find the ASCII value of characterc. Then divide the number by n=15 and print

the exact result.

*/

int main()
{
	char c = 'a';
	double n = 15;

	int cInt = (int) c;

	double valueFinal = cInt/n;

	printf("%lf\n", valueFinal);
}
