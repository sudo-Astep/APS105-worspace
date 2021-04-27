#include <stdio.h>

void swap(int*, int*, int*);

int main()
{
int a=1, b=2, c=3;
printf("%d %d %d\n", a, b, c);

/* Call the function swap below this line. */
swap(&a, &b, &c);


printf("%d %d %d\n", a, b, c);

return 0;
}

/* Write the function swap below this line. */
void swap(int* a, int* b, int* c)
{
	int temp = *a;

	*a = *c;
	*c = *b;
	*b = temp;
}
