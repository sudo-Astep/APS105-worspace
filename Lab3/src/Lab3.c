#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Lab3.c
 *
 *  Created on: Jan 22, 2021
 *
 *  Author: Andrija Stepanovic
 *  Student ID: 1007107239
 *
 *	Plays a simplified game of Blackjack against the computer using pseudo-random numbers.
 */

int main (int argc, char **argv)
{
	// Variable declarations
	int playerCardWorth = 0; // Total worth of the player's cards
	int dealerCardWorth = 0; // Total worth of the dealer's cards
	int newPlayerCard; // The worth of the player's newest card
	int newDealerCard; // The worth of the dealer's newest card
	int playerChoice; // The choice the player makes when playing, hit or stay

	// Sets pseudo-random seed
	if (argc == 1)
		srand (time (NULL));
	else
		srand (atoi (argv[1]));

	// Gives player and dealer first card
	newPlayerCard = (rand () % 13) + 1;
	newDealerCard = (rand () % 13) + 1;

	// Prints the player's and dealer's first cards
	printf ("First cards: player %d dealer %d\n", newPlayerCard, newDealerCard);

	// Logic for adding 10 points for cards that are 10 or above
	if (newPlayerCard >= 10)
	{
		playerCardWorth += 10;
	}
	else
	{
		playerCardWorth += newPlayerCard;
	}

	if (newDealerCard >= 10)
	{
		dealerCardWorth += 10;
	}
	else
	{
		dealerCardWorth += newDealerCard;
	}

	// Loop that gives player cards as long as they hit or until their card worth hits more than 21
	do
	{
		// Loop handles incorrect input, prompts user again if input is incorrect
		do
		{
			// Player input for their choice
			printf ("Type 1 for Hit, 0 to Stay:");
			scanf ("%d", &playerChoice);
		} while (playerChoice != 1 && playerChoice != 0);

		// If player hits, they get a card
		if (playerChoice == 1)
		{
			// Gives player a card
			newPlayerCard = (rand () % 13) + 1;

			// Logic for adding 10 points for cards that are 10 or above
			if (newPlayerCard >= 10)
			{
				playerCardWorth += 10;
			}
			else
			{
				playerCardWorth += newPlayerCard;
			}

			// Prints playe's new card and current card worth
			printf ("Player gets a %d, worth is %d\n", newPlayerCard,
					playerCardWorth);
		}

	} while (playerCardWorth <= 21 && playerChoice == 1);

	// If player's card worth is above 21, they automatically lose
	if (playerCardWorth > 21)
	{
		printf ("Player over 21, Dealer wins\n");
		return 0;
	}

	printf ("Dealer gets:");

	// Gives dealer cards until the card worth is above 17
	do
	{
		// Gives dealer new card
		newDealerCard = (rand () % 13) + 1;

		// Prints the card the dealer got
		printf (" %d", newDealerCard);

		// Logic for adding 10 points for cards that are 10 or above
		if (newDealerCard >= 10)
		{
			dealerCardWorth += 10;
		}
		else
		{
			dealerCardWorth += newDealerCard;
		}

	} while (dealerCardWorth <= 17);

	printf ("\n");

	// Dealer automatically loses if card worth is above 21
	if (dealerCardWorth > 21)
	{
		printf ("Dealer over 21, Player wins\n");
		return 0;
	}

	// If cards are equal, it's a tie
	if (dealerCardWorth == playerCardWorth)
	{
		printf ("Tie!\n");
		return 0;
	}

	// If dealer has a higher card worth they win
	if (dealerCardWorth > playerCardWorth)
	{
		printf ("Dealer better than Player, Dealer wins\n");
		return 0;
	}

	// If player has higher card worth, they win
	if (dealerCardWorth < playerCardWorth)
	{
		printf ("Player better than Dealer, Player wins\n");
		return 0;
	}

	// If the code gets here there was an error, hence the 1
	return 1;
}

