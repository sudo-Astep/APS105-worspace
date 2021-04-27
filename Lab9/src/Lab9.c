#include <stdio.h>
#include <stdlib.h>
#include "contactList.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// IF IT'S EASIER, YOU NEED NOT BE OVERLY GENERAL/FLEXIBLE
// BUT CAN ASSUME THESE LIMITS
#define MAXCONTACTS 30 /* max # of contacts */
#define MAXPHONES 10 /* max # of phone numbers for each contact */

#define PHONE_NUMBER_MAX_LENGTH 12

// Struct that stores the contact information
typedef struct info
{
	char name[128]; // The name of the contact
	long numbers[16]; // The contact numbers
	int numNumbers; // The number of numbers the contact has

	struct info *next; // The next element in the linked list
} contact;

contact *head; // The begining of the linked list

// INCLUDE YOUR OWN "HELPER" ROUTINES AS YOU SEE FIT

/*
 * Function: listContacts
 * ----------------------------
 * Lists all the contacts and their phone numbers in alphabetical order
 */
void ListAllContacts ()
{
	contact *current = head;

	// Cycles through all the elements of the linked list and prints the contents
	while (current != NULL)
	{
		// Prints name
		printf ("Name: %s\n", current->name);

		// Prints all the phone numbers for the contact in reverse order
		// FOR SOME REASON EXAMIFY WANTS IT THIS WAY, IT'S VERY STUPID
		printf ("Phone(s):");
		for (int i = current->numNumbers - 1; i >= 0; i--)
		{
			printf (" %ld", current->numbers[i]);
		}

		printf ("\n");

		current = current->next;
	}

}

/*
 * Function: compareNames
 * ------------------------
 * Compares two strings, returns +ve if name1 is alphabetically higher, -ve otherwise
 * Returns 0 if the strings are the same
 */
int compareNames (char *name1, char *name2)
{
	int i = 0;
	// Goes though each character of the string, until one of the strings reaches the end
	for (i = 0; !((name1[i] != '\0') ^ (name2[i] != '\0')); i++)
	{
		// Returns the difference in the character on the first different character
		if (name1[i] > name2[i])
			return name1[i] - name2[i];

		if (name1[i] < name2[i])
			return name1[i] - name2[i];

		// If the strings are the same and end at the same time, they are identical
		if (name1[i] == '\0' && name2[i] == '\0')
			return 0;
	}

	// If one of the strings ends before the other, the longer one is alphabetically higher
	return name1[i] - name2[i];
}

/*
 * Function: insertContact
 * -------------------------
 * Creates new struct for data and imediately sorts it into it's propper place
 * using Insertion Sort
 */
void insertContact (char name[128], long phoneNumbers[16], int numNumbers)
{
	// Allocates memory for new struct
	contact *new = (contact*) malloc (sizeof(contact));

	contact *current = head;
	contact *previous = NULL;

	// Loads data into new struct
	strcpy (new->name, name);
	memcpy (new->numbers, phoneNumbers, sizeof(long) * 16);
	new->numNumbers = numNumbers;

	// If head is empty sets new struct as the head
	if (strcmp (current->name, "") == 0)
	{
		head = new;
		return;
	}

	// If the new name is before the first name, new name becomes the first name
	if (compareNames (name, current->name) < 0)
	{
		new->next = head;
		head = new;
		return;
	}

	// Sets values for the iteration of the loop
	if (current->next != NULL)
	{
		previous = current;
		current = current->next;
	}

	// Goes through all the elements in the linked list
	while (current->next != NULL)
	{
		// If the new struct name is greater than one element and less than the one after it, it should be in between them
		if (compareNames (name, current->name) < 0 && compareNames (name, previous->name) > 0)
		{
			previous->next = new;
			new->next = current;
			return;
		}

		previous = current;
		current = current->next;
	}

	// Checks for last element if the new element should be before or after it
	if (compareNames (name, current->name) > 0)
	{
		new->next = NULL;
		current->next = new;
	}
	else
	{
		new->next = current;
		previous->next = new;
	}

}

/*
 * convertInputToDataStructure
 * Takes the information in contactList.h and puts it into a data
 * structure of your choosing, however: 1 mark of the lab will be
 * for using structs with pointers in your solution.
 * You might also want to consider the outputs from the program
 * (see the menu printed in printMenu) since some structures will
 * make this easier than others.
 */
void convertInputToDataStructure ()
{
	int numNumbers= 0; // Counter, number of numbers for a struct

	char name[128]; // Name
	long phoneNumbers[16] =
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Numbers array, initialized so all numbers are 0

	// Iterates through each element in the provided array
	for (int i = 0; contactList[i] != NULL; i++)
	{
		// sets the first name without inserting new element into list
		if (contactList[i][0] >= 'A' && i == 0)
		{
			strcpy (name, contactList[i]);
		}
		// If the current element is a number, it gets added to the numbers array
		else if (contactList[i][0] < 'A')
		{
			char *endptr;
			phoneNumbers[numNumbers] = strtol (contactList[i], &endptr, 0);

			numNumbers++;
		}
		// If the element is a name, the previous contact gets added to the linked list and new name is set
		else if (contactList[i][0] >= 'A')
		{
			// Inserts contact to list
			insertContact (name, phoneNumbers, numNumbers);

			// Copies new name
			strcpy (name, contactList[i]);

			// Resets counter
			numNumbers = 0;

			// Placeholder array
			long newNumbers[16] =
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			// resets numbers array
			memcpy (phoneNumbers, newNumbers, sizeof(long) * 16);
		}
	}

	// Inserts final contact
	insertContact (name, phoneNumbers, numNumbers);
}

/*
 * printMenu - Prints out the user menu
 */
void printMenu ()
{
	printf ("\n\nEnter a command by number\n");
	printf ("1. List all contacts in alphabetical order\n");
	printf ("2. Print phone(s) for a contact\n");
	printf ("3. Find duplicate entries\n");
	printf ("4. Delete contact\n");
	printf ("5. Exit the program\n");
	printf ("Your input: ");
}

//eliminate trailing \n
char* rtrim (char *str)
{
	int i;
	for (i = strlen (str) - 1; i >= 0 && str[i] == '\n'; i--)
		str[i] = '\0';
	return str;
}

/*
 * Function: PrintContact
 * -------------------------
 * Prints the name of one contact that the user chooses
 */
void PrintContact ()
{
	char input[128];// User input
	contact *current;

	// Gets user input
	printf ("Enter name: ");
	char *name = fgets (input, sizeof(input), stdin);

	name = rtrim(name);

	current = head;

	// Goes through each linked list element and checks if it's the one being searched for
	while (current != NULL)
	{
		// If the contact is found
		if (compareNames (current->name, name) == 0)
		{
			// Prints name
			printf ("Name: %s", name);
			printf ("Phone(s):");

			// Prints all the phone numbers
			// THIS TIME NOT IN REVERSE ORDER FOR SOME REASON???
			for (int i = 0; i < current->numNumbers; i++)
			{
				printf (" %ld", current->numbers[i]);
			}

			printf ("\n");

			return;
		}

		current = current->next;
	}

	printf ("Contact not found\n");
}

/*
 * DeleteContact deletes a contact from the list
 * The user must enter the name of the contact to delete
 *
 */

void DeleteContact ()
{
	char input[128]; // User input
	contact *current;
	contact *previous = NULL;

	// Gets user input
	printf ("Enter name: ");
	char *name = fgets (input, sizeof(input), stdin);

	name = rtrim(name);

	current = head;

	// Iterates through each element in the list
	while (current != NULL)
	{
		// If a match is found the contact is deleted
		if (compareNames (current->name, name) == 0)
		{
			// Removes the node from the list
			if (previous != NULL)
			{
				previous->next = current->next;
			}
			else
			{
				head = current->next;
			}

			free(current);

			return;
		}

		previous = current;
		current = current->next;
	}

	printf ("Contact not found\n");
}

/*
 * findDuplicates finds the names of contacts with
 * a phone number in common
 */

void FindDuplicates ()
{
	contact *current = head;

	bool dupsFound = false;

	// Iterates through all nodes in the list
	while (current != NULL)
	{
		contact *checking = current->next;

		// goes through all numbers in the current node
		for (int i = 0; i < current->numNumbers; i++)
		{
			// Goes through all elements after current element
			while (checking != NULL)
			{
				// Goes through all the numbers in the checking node
				for (int j = 0; j < checking->numNumbers; j++)
				{
					// Prints message if match it found
					if (current->numbers[i] == checking->numbers[j])
					{
						printf ("%s and %s have a phone number in common\n", current->name, checking->name);
						dupsFound = true;

						break;
					}
				}

				checking = checking->next;
			}
		}

		current = current->next;
	}

	// Prints message if no duplicates are found
	if (!dupsFound)
		printf ("No duplicates found\n");
}

int main ()
{
	head = (contact*) malloc (sizeof(contact));
	// first move data from contactList.h to a data structure
	convertInputToDataStructure ();

	char notDone = 1;
	while (notDone)
	{
		printMenu ();
		char userChoice[100];
		char *s = fgets (userChoice, sizeof(userChoice), stdin);
		if (!s)
		{
			printf ("Wrong input\n");
			continue;
		}
		s = rtrim (s); /* eliminate trailing \n */
		//printf("selection = %s\n", s);
		int userSelection = atoi (s);

		switch (userSelection)
		{
			case 1:
				ListAllContacts ();
				break;
			case 2:
				PrintContact ();
				break;
			case 3:
				FindDuplicates ();
				break;
			case 4:
				DeleteContact ();
				break;
			case 5:
				notDone = 0;
				break;
			default:
				printf ("Bad command...\n");
				break;
		}

	}

	// PUT ANY POST_RUN CODE HERE

	return 0;
}
