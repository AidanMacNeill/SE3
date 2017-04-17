/*
 * main.c
 *
 *  Created on: 9 Apr 2017
 *      Author: Aidan
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "crossfireOperations.h"

int deadPlayers; // Global int to count the number of dead players in the game

int main(void)
{

	setbuf(stdout,NULL); // Allow scanf to work with Eclipse
	srand(time(NULL)); // Seed rand function
	int numPlayers; // Stores number of players in game

	struct slot ** board = calloc(BOARD_SIZE, sizeof(struct slot *));

	for(int i = 0; i < BOARD_SIZE; i++)
	{
		board[i] = calloc(BOARD_SIZE, sizeof(struct slot)); // Allocate space in memory for each slot
	}

	createBoard(board, BOARD_SIZE); // Initialise board

	printf("\nThere can be 2-%d players in the game.\n", PLAYER_MAX);
	printf("How many players are there: ");
	scanf("%d", &numPlayers);
	while(numPlayers < 2 || numPlayers > PLAYER_MAX) // Simple error checking
	{
		printf("\nThat number of players is invalid. Please enter a valid number of players.\n");
		scanf("%d", &numPlayers);
	}

	struct player_type *player;
	player = calloc(numPlayers, sizeof(struct player_type)); // Creating player struct with malloc

	printf("\nA player's name can have a maximum length of 19 characters.\n");
	printf("There are 4 types of player: Elf, Human, Ogre and Wizard.\n");	//Giving information to user on the possible types of player_type and name specs.
	for(int i = 0; i < numPlayers; i++)
		playerInit(board, player, i); // Set up all players
	playcrossFire(board, player, numPlayers, deadPlayers); // Play the game
}
