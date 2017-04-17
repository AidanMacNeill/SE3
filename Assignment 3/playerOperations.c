/*
 * playerOperations.c
 *
 *  Created on: 9 Apr 2017
 *      Author: Aidan
 */

#include "crossfireOperations.h"
#include <stdio.h>
#include <string.h>

void MoveSlot (struct slot ** board,struct player_type * player, const int numOfPlayers, int row, int column, int i)
{
	//Storing players previous position
	int tempRow = player[i].row;
	int tempColumn = player[i].column;

	//Assigning new position to player
	player[i].row = row;
	player[i].column = column;

	//Indicating in moved to slot that it is occupied. Adding player to presentPlayers
	board[row][column].occupied = true;
	board[row][column].playersPresent[player[i].id] = 1;	//Indicating user is present in slot by setting the corresponding index in the array to their id = 1;

	//Removing player from previous slot playersPresent
	board[tempRow][tempColumn].playersPresent[player[i].id] = 0;

	int k;
	//Checking if their are any players in the slot.
	for(k = 0; k < numOfPlayers; k++)
	{
		//If a player is present, exit loop.
		if(board[tempRow][tempColumn].playersPresent[k] == 1)
		{
			break;
		}
	}
	// After the for loop, if k = total number of players then no players are present in that slot. Set slot occupied to false.
	if(k == numOfPlayers)
	{
		board[tempRow][tempColumn].occupied = false;
	}

	// Apply stat changes based off new slots
	if(board[player[i].row][player[i].column].slot_type == cityType)
	{
		playeronCity(&player[i]);
	}
	else if(board[player[i].row][player[i].column].slot_type == hillType)
	{
		playeronHill(&player[i]);
	}
}

void playerQuit(struct slot ** board,struct player_type * player)
{
	board[player -> row][player -> column].playersPresent[player -> id] = 0;// Removing player from current slot

	extern int deadPlayers; // Variable to hold number of players who died or left the game
	int i;
	// Checking if their are any players in the slot
	for(i = 0; i < PLAYER_MAX; i++)
	{
		//If a player is present, exit loop.
		if(board[player->row][player->column].playersPresent[i] == 1)
		{
			break;
		}
	}
	//After the for loop, if i = total number of players then no players are present in that slot. Set slot occupied to false.
	if(i==PLAYER_MAX)
	{
		board[player->row][player->column].occupied = false;
	}

	//Setting player off the board and dead
	player->lifepoints = 0;
	player->row = -1;
	player->column = -1;
	deadPlayers++;	//Incrementing number of players dead/quit
}

void playerAttackCheck(struct slot ** board, struct player_type * player, int * checkerD, int x, int * playersFound)
{
	//Resetting checks of possible attacks to false
	player[x].nearCheck = false;
	player[x].distantCheck = false;
	player[x].magicCheck = false;

	//Resetting checkerD to false
	*checkerD = 0;

	for(int k = 0; k < BOARD_SIZE; k++)
	{
		for(int l = 0; l < BOARD_SIZE; l++)
		{
			board[k][l].explored = false;
		}
	}
	//Resetting playersFound array to 0
	for(int i = 0; i < PLAYER_MAX; i++)
	{
		playersFound[i]=0;
	}

	//Checking if a near attack is possible and assigning result to player's check.
	player[x].nearCheck = checkNearAttack(board, player[x].row, player[x].column, x, playersFound);

	//checking if a distant attack is possible
	for(int i = 2; i < 5; i++)
	{
		findSlots(i, 0,  &board[player[x].row][player[x].column], playersFound, checkerD, x);
	}
	//Setting distant check to true if there is another player found
	if((*checkerD) > 0)	//1 because the player always finds them-self
	{
		player[x].distantCheck = true;
	}

	if((player[x].smartness + player[x].magicskills) > 150) // Checking if player can make a magic attack
	{
		player[x].magicCheck = true;
	}
}

void playerMoveChoice(struct slot ** board, struct player_type * player, char *slotChoice, int n, int i)
{
	//Printing which slots the player can move to.
	printf("What slot would you like to move to:\n");
	if((board[player[i].row][player[i].column].up)!=NULL){
		printf("w - for slot above\n");
	}
	if((board[player[i].row][player[i].column].down)!=NULL){
		printf("s - for slot below\n");
	}
	if((board[player[i].row][player[i].column].left)!=NULL){
	printf("a - for slot to the left\n");
	}
	if((board[player[i].row][player[i].column].right)!=NULL){
		printf("d - for slot to the right\n?");
	}

	do{
		//Prompting user for which slot they would like to  move to
		printf("Choice: ");
		fflush(stdin);
		scanf(" %c", &(*slotChoice));

		switch(*slotChoice)
		{
			case 's':	//Player wants to move down
				//Checking if the chosen slot exists
				if((board[player[i].row][player[i].column].down)!=NULL){
					//Moving player
					MoveSlot(board, player, n, player[i].row+1, player[i].column, i);
					printf("\n\n%s (Player %d) has moved to slot (%d,%d)\n\n",player[i].name, i+1, player[i].row, player[i].column);
					break;
				}
				else{	//If slot chosen does not exist, alert user and set slotChoice so that they get prompted to choose a different slot
					printf("Incorrect value entered!\n");
					*slotChoice = -1;
					break;
				}
			case 'w':	//Player wants to move up
				//Checking if the chosen slot exists
				if((board[player[i].row][player[i].column].up)!=NULL){
					//Moving player
					MoveSlot(board, player, n, player[i].row-1, player[i].column, i);
					printf("\n\n%s (Player %d) has moved to slot (%d,%d)\n\n",player[i].name, i+1, player[i].row, player[i].column);
					break;
				}
				else{	//If slot chosen does not exist, alert user and set slotChoice so that they get prompted to choose a different slot
					printf("Incorrect value entered!\n");
					*slotChoice = -1;
					break;
				}
			case 'a':	//Player wants to move left
				//Checking if the chosen slot exists
				if((board[player[i].row][player[i].column].left)!=NULL){
					//Moving player
					MoveSlot(board, player, n, player[i].row, player[i].column-1, i);
					printf("\n\n%s (Player %d) has moved to slot (%d,%d)\n\n",player[i].name, i+1, player[i].row, player[i].column);
					break;
				}
				else{	//If slot chosen does not exist, alert user and set slotChoice so that they get prompted to choose a different slot
					printf("Incorrect value entered!\n");
					*slotChoice = -1;
					break;
				}
			case 'd':	//Player wants to move right
				//Checking if the chosen slot exists
				if((board[player[i].row][player[i].column].right)!=NULL){
					//Moving player
					MoveSlot(board, player, n, player[i].row, player[i].column+1, i);
					printf("\n\n%s (Player %d) has moved to slot (%d,%d)\n\n",player[i].name, i+1, player[i].row, player[i].column);
					break;
				}
				else{	//If slot chosen does not exist, alert user and set slotChoice so that they get prompted to choose a different slot
					printf("Incorrect value entered!\n");
					*slotChoice = -1;
					break;
				}
			default:	//Invalid value was entered. Alert user and set slotChoice so that they get prompted to choose a different slot
				printf("Incorrect value entered!\n");
				*slotChoice = -1;
				break;
		}
	}while(*slotChoice == -1);	//Continues until a valid slot is chosen to move to
}

void playerInit(struct slot ** board, struct player_type * player, int i)
{
	int playerChoice;	// Variable to hold player type
	printf("\nEnter Player %d's name: ", i+1);
	scanf("%19s", player[i].name);	// Max size of 19 due to null termination

	printf("\n----------Enter Player %d's type---------\n", i+1);
	printf("1 - Human, 2 - Ogre, 3 - Wizard, 4 - Elf:\n");
	scanf("%d", &playerChoice);		// User input for player type

	while(playerChoice < 1 || playerChoice > 4)
	{	// Loops while entered digit is not a valid type
		printf("Sorry but you must enter a number between 1 & 4:\n");
		scanf("%d", &playerChoice);
	}
	player[i].lifepoints = 100.0;		// Sets hp to 100 as stated in brief

	player[i].id = i;	//Setting player's ID
	/* Switch that copies player's type and respective capabilities to each struct member */
	switch(playerChoice - 1)
	{
		case humanType:
			strcpy(player[i].type, "Human");
			playertypeHuman(&player[i], board);		// Elf function
			break;
		case ogreType:
			strcpy(player[i].type, "Ogre");
			playertypeOgre(&player[i], board);		// Human function
			break;
		case wizardType:
			strcpy(player[i].type, "Wizard");
			playertypeWizard(&player[i], board);		// Ogre function
			break;
		case elfType:
			strcpy(player[i].type, "Elf");
			playertypeElf(&player[i], board);		// Wizard function
			break;
	}

	if(board[player[i].row][player[i].column].slot_type == cityType) // Edit player stats based on their current position
	{
		playeronCity(&player[i]);
	}
	else if(board[player[i].row][player[i].column].slot_type == hillType)
	{
		playeronHill(&player[i]);
	}
}

void nearAttack(struct player_type *attacker, struct player_type *attacked, struct slot ** board)
{
	if(attacked->strength <= 70) // Attacks as stated in brief
	{
		attacked->lifepoints -= (0.5 * attacker->strength);
		if(attacked->lifepoints<=0)
		{
			printf("%s, player %d, has been killed by %s, player %d, and has been removed from play.\n",attacked->name,attacked->id,attacker->name,attacker->id);
			playerQuit(board, attacked);
		}
	}
	else if(attacked->strength > 70) // Attacks as stated in brief
	{
		attacker->lifepoints -= (0.3 * attacked->strength);
		if(attacker->lifepoints<=0)
		{
			printf("%s, player %d, has been killed by %s, player %d, and has been removed from play.\n",attacker->name,attacker->id,attacked->name,attacked->id);
			playerQuit(board, attacker);
		}
	}
}

void distantAttack(struct player_type *attacker, struct player_type *attacked, struct slot ** board)
{
	float x = attacker->strength;
	if(attacker->dexterity > attacked->dexterity) // Attacks as stated in brief
	{
		attacked->lifepoints -= (0.3 * x);
		if(attacked->lifepoints<=0)
		{
			printf("%s, player %d, has been killed by %s, player %d, and has been removed from play.\n",attacked->name,attacked->id,attacker->name,attacker->id);
			playerQuit(board, attacked);
		}
	}
}

void magicAttack(struct player_type *attacker, struct player_type *attacked, struct slot ** board)
{
	attacked->lifepoints -= (0.5 * attacker->magicskills)+(0.2 * attacker->smartness); // Attacks as stated in brief
	if(attacked->lifepoints<=0)
	{
		printf("%s, player %d, has been killed by %s, player %d, and has been removed from play.\n",attacked->name,attacked->id,attacker->name,attacker->id);
		playerQuit(board, attacked);
	}
}


void printPlayersStats(struct player_type *player, int n, struct slot ** board) // Function to print player stats
{
	for(int i = 0; i < n; i++)	// Loop through all players
	{
		if(player[i].lifepoints>0)
		{
			printf("\n-PLAYER %d-\n", i+1);
			printf("Name:\t\t%s\n", player[i].name);
			printf("Type:\t\t%s\n", player[i].type);
			printf("Life Points:\t%.1f\n", player[i].lifepoints);
			printf("Smartness:\t%d\n", player[i].smartness);
			printf("Strength:\t%d\n", player[i].strength);
			printf("Magic Skills:\t%d\n", player[i].magicskills);
			printf("Luck:\t\t%d\n", player[i].luck);
			printf("Dexterity:\t%d\n", player[i].dexterity);
			printf("Slot Name:\t%s\n", board[player[i].row][player[i].column].slot_name);
			printf("Player ID:\t%d\n",player[i].id);
			printf("Position:\t(%d,%d)\n",player[i].row,player[i].column);
		}
	}
}


