//put include statements here
#include <stdio.h>
#include <stdbool.h>

void loadPrimes(int[]);
bool isPrime(int);

int main(int argc, char **argv)
{
	int primes[30];

	loadPrimes(primes);

    int primeWanted=2;

    while(1){ //make sure you don't infinite loop and that you return correctly from main

        printf("Which prime should be printed? ");
  	   printf("(1-30, non-positive input terminates the program):");
        scanf("%d",&primeWanted);

       // finish the program here, including completing this output statement:
        if(primeWanted < 31 && primeWanted > 0)
        	printf("Prime #%d is %d\n", primeWanted, primes[primeWanted - 1]);
        if(primeWanted <= 0)
        {
        	return 0;
        }
    }
}

void loadPrimes(int primes[])
{
	int i = 1;

	*primes = 2;

	for(int j = 3; i < 30; j += 2)
	{
		if(isPrime(j))
		{
			*(primes + i) = j;
			i++;
		}
	}
}

bool isPrime(int n)
{
	for (int i = 2; i * i <= n; i++)
	{
		if(n % i == 0)
			return false;
	}

	return true;
}
