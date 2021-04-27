#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * SortingAlgorithms.c
 *
 *  Created on: Apr 27, 2021
 *      Author: ace
 */

void bubbleSort(int[], int);
void insertionSort(int[], int);
void selectionSort(int[], int);
void quickSort(int[], int, int);
void mergeSort(int[], int, int);
int partition(int [], int, int);
void merge (int [], int, int, int);

int main ()
{
	srandom(time(NULL));

	int n = 0;
	int maxNum = 10;
	char sort;

	scanf("%d", &n);
	scanf ("%d", &maxNum);

	int numbers[n];

	printf ("Unsorted array:\n");

	for (int i = 0; i < n; i++)
	{
		numbers[i] = random() % maxNum;
		printf ("%d ", numbers[i]);
	}
	printf ("\n\n");

	scanf(" %c", &sort);

	switch (sort)
	{
		case 'b':
			bubbleSort(numbers, n);
			break;

		case 's':
			selectionSort (numbers, n);
			break;

		case 'q':
			quickSort (numbers, 0, n);
			break;

		case 'i':
			insertionSort (numbers, n);
			break;

		case 'm':
			mergeSort (numbers, 0, n);
			break;

		default:
			printf ("Unknown algorithm\n");
			exit(1);
	}

	printf ("Sorted array:\n");
	for(int i = 0; i < n; i++)
	{
		printf ("%d ", numbers[i]);
	}
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
		int mid = (low + high)/2;

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

	for (int i = 0; i < size2; i++)
	{
		highArr[i] = numbers[mid + i + 1];
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

















