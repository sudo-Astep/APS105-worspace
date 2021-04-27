#include <stdio.h>

int main(void)
{
	int row, col, num;

	printf("Enter the number of rows in the triangle: ");
	scanf("%d", &num);

	while(num >= 0)
	{
		for(int i = 0; i < (2*num) - 1; i++)
		{
				printf("*");
		}

		printf("\n");

		for(int i = num - 2; i >= 0; i--)
			{
				// Prints first * with appropriate spacing
				printf("%*s", num - i, "*");

				// Prints the second * with appropriate spacing
				if(i != 0)
					printf("%*s", (2 * i), "*");

				printf(" \n");
			}

		printf("Enter the number of rows in the triangle: ");
        scanf("%d", &num);
	}

  printf("Goodbye\n\n");

  return 0;
}
