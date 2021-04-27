#include <stdio.h>

int main (void)
{
	int Sum = 0;

	for(int i = 1; i <= 1000; i++)
	{
		if(i % 3 == 0)
		{
			if(i % 2 != 0)
			{
				if((i % 100) / 10 != 5)
					Sum += i;
			}
		}
	}

	printf("Sum of Required Numbers is %d\n",Sum);
}
