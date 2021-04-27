#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
 * SearchingAlgorithms.c
 *
 * Implementation of searching algorithms:
 * 		- Linear Search
 * 		- Binary Search
 *
 * 			***NOTE***
 * Binary search requires that the list first be sorted. Here I use quick sort
 * beacuse it's my favorite. Others can also be used and the code can be changed
 * easily enough to reflect that.
 *
 *  Created on: Apr 27, 2021
 *      Author: ace
 */

bool linearSearch (int[], int, int);
bool binarySearch (int[], int, int, int);
void bubbleSort(int[], int);
void insertionSort(int[], int);
void selectionSort(int[], int);
void quickSort(int[], int, int);
void mergeSort(int[], int, int);
int partition(int [], int, int);
void merge (int [], int, int, int);

int main ()
{
	/*
	 * NOTE!
	 *
	 * random might not work on some machines, if that's the case for you
	 * use rand and srand instead of random and srandom
	 */
	srandom(time(NULL));

	int n = 0;
	int maxNum = 10;
	char search;
	int key;

	printf ("Enter the number of elements in the array: ");
	scanf("%d", &n);

	printf ("Enter the maximum number that appears in the array: ");
	scanf ("%d", &maxNum);

	int numbers[n];

	printf ("Array:\n");

	for (int i = 0; i < n; i++)
	{
		numbers[i] = random() % maxNum;
		printf ("%d ", numbers[i]);
	}
	printf ("\n\n");

	printf ("What searching method to use (first letter of the name in lower case): ");
	scanf (" %c", &search);

	printf ("Number to find: ");
	scanf ("%d", &key);

	switch (search)
	{
		case 'b':
			quickSort (numbers, 0, n - 1); // IF YOU WANT TO CHANGE THE SORTING ALGORITHM, JUST CHANGE THIS LINE, ALL THE ALGORITHMS ARE IN THE FILE ALREADY
			if (binarySearch(numbers, key, 0, n - 1))
				printf ("The number %d is in the array!", key);
			else
				printf ("The number %d is not in the array", key);
			break;

		case 'l':
			if (linearSearch(numbers, n, key))
							printf ("The number %d is in the array!", key);
						else
							printf ("The number %d is not in the array", key);
			break;

		default:
			printf ("Unknown algorithm\n");
			return 1;
	}


	return 0;
}

bool linearSearch (int numbers[], int n, int key)
{
	for (int i = 0; i < n; i++)
	{
		if (numbers[i] == key)
			return true;
	}

	return false;
}

bool binarySearch (int numbers[], int key, int low, int high)
{
	int mid = low + (high-low)/2;

	if (low == high && numbers[low] != key)
		return false;

	if (numbers[mid] == key)
		return true;

	if (numbers[mid] < key)
	{
		return binarySearch(numbers, key, mid + 1, high);
	}
	else if (numbers[mid] > key)
	{
		return binarySearch (numbers, key, low, mid - 1);
	}

	return false;
}


void bubbleSort(int numbers[], int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (numbers[j] > numbers[j + 1])
			{
				int temp = numbers[j];
				numbers[j] = numbers[j + 1];
				numbers[j + 1] = temp;
			}
		}
	}
}


void selectionSort (int numbers[], int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		int min = numbers[i];
		int minIndex;

		for (int j = i; j < n; j++)
		{
			if (numbers[j] < min)
			{
				min = numbers[j];
				minIndex = j;
			}
		}

		numbers[minIndex] = numbers[i];
		numbers[i] = min;
	}
}

void insertionSort (int numbers[], int n)
{
	for (int i = 1; i < n; i++)
	{
		int curr = numbers[i];

		int j = i - 1;

		while (j >= 0 && curr < numbers[j])
		{
			numbers[j + 1] = numbers[j];

			j--;
		}

		numbers[j + 1] = curr;
	}
}

void quickSort (int numbers[], int low, int high)
{
	if (low < high)
	{
		int pivot = partition(numbers, low, high);

		quickSort(numbers, low, pivot - 1);
		quickSort(numbers, pivot + 1, high);
	}
}

void mergeSort (int numbers[], int low, int high)
{
	if (low < high)
	{
		int mid = low + (high - low)/2;

		mergeSort (numbers, low, mid);
		mergeSort (numbers, mid + 1, high);

		merge (numbers, low, mid, high);
	}
}


void merge (int numbers[], int low, int mid, int high)
{
	int size1 = mid - low + 1;
	int size2 = high - mid;

	int lowArr[size1];
	int highArr[size2];

	for (int i = 0; i < size1; i++)
	{
		lowArr[i] = numbers[low + i];
	}

	for (int j = 0; j < size2; j++)
	{
		highArr[j] = numbers[mid + j + 1];
	}

	int i = 0;
	int j = 0;

	int c = low;

	while (i < size1 && j < size2)
	{
		if (lowArr[i] <= highArr[j])
		{
			numbers[c] = lowArr[i];
			i++;
		}
		else
		{
			numbers[c] = highArr[j];
			j++;
		}
		c++;
	}

	while (i < size1)
	{
		numbers[c] = lowArr[i];
		i++;
		c++;
	}

	while (j < size2)
	{
		numbers[c] = highArr[j];
		j++;
		c++;
	}
}


int partition (int numbers[], int low, int high)
{
	int pivot = numbers[high];
	int i = low - 1;

	for (int j = low; j < high; j++)
	{
		if (numbers[j] < pivot)
		{
			i++;

			int temp = numbers[i];
			numbers[i] = numbers[j];
			numbers[j] = temp;
		}
	}

	int temp = numbers[i + 1];
	numbers[i + 1] = numbers[high];
	numbers[high] = temp;

	return i + 1;
}

























