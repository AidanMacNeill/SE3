/*
 * userInteraction.c

 *
 *  Created on: 9 Apr 2017
 *      Author: Aidan
 */

#include "crossfireOperations.h"
#include <stdio.h>
#include <stdlib.h>


void playertypeHuman(struct player_type *player, struct slot **board){
	int attributes[5], sum, i; // Array to hold each attribute
	do
	{
		sum = 0;
		for(i = 0; i < 5; i++)
		{
			attributes[i] = rand() % 100 + 1;	// Each attribute is assigned a random value from 1-100 inclusive.
			sum += attributes[i];				// Use a sum so attribute cap isn't exceeded
		}
	}while(sum >= 300);

	player->smartness = attributes[0]; // Assign attributes to player
	player->strength = attributes[1];
	player->magicskills = attributes[2];
	player->luck = attributes[3];
	player->dexterity = attributes[4];
	player->row = rand() % BOARD_SIZE; // Assign position to user
	player->column = rand() % BOARD_SIZE;
	board[player->row][player->column].occupied = true;
	board[player->row][player->column].playersPresent[player->id] = 1;
}

void playertypeOgre(struct player_type *player, struct slot **board){
	int attributes[2], sum; // Array to hold each attribute

	do
	{
		sum = 0;
		attributes[0] = rand() % 21; // Smartness
		attributes[1] = rand() % 51; // Luck
		sum = attributes[1] + attributes[0];		//Calculating sum of both attributes.
	}
	while(sum > 50);

	player->smartness = attributes[0]; // Assign attributes to player
	player->luck = attributes[1];
	player->strength = rand() % 21 + 80;
	player->dexterity = rand() % 21 + 80;
	player->magicskills = 0;
	player->row = rand() % BOARD_SIZE;
	player->column = rand() % BOARD_SIZE;
	board[player->row][player->column].occupied = true;
	board[player->row][player->column].playersPresent[player->id]=1;
}


void playertypeWizard(struct player_type *player, struct slot **board){

	player->smartness = rand() % 11 + 90;
	player->luck = rand() % 51 + 50;
	player->strength = rand() % 20 + 1;
	player->dexterity = rand() % 100 + 1;
	player->magicskills = rand() % 21 + 80;
	player->row = rand() % BOARD_SIZE;
	player->column = rand() % BOARD_SIZE;
	board[player->row][player->column].occupied = true;
	board[player->row][player->column].playersPresent[player->id]=1;
}

void playertypeElf(struct player_type *player, struct slot **board){

	player->smartness = rand() % 31 + 70;
	player->luck = rand() % 41 + 60;
	player->strength = rand() % 50 + 1;
	player->dexterity = rand() % 100 + 1;
	player->magicskills = rand() % 29 + 51;
	player->row = rand() % BOARD_SIZE;
	player->column = rand() % BOARD_SIZE;
	board[player->row][player->column].occupied = true;
	board[player->row][player->column].playersPresent[player->id]=1;
}

void playcrossFire(struct slot **board, struct player_type * player, int n, int deadPlayers){

// Global integer to hold the number players who are out if the game
	deadPlayers = 0;
	int playersFound[PLAYER_MAX] = {0}, checkerD, i, j, choice;
	char slotChoice;

	int allowedChoice;
	int roundNum=0;
	int playerRound;

	do
	{
		printPlayersStats(player, n, board);
		printf("\n\n-----ROUND %d-----\n", roundNum+1);
		playerRound = 0;
		for(int i=0;i<n;i++)	//Loop through players
		{
			playerRound++;
			if(player[i].lifepoints>0)	//If player is alive allow them to take their turn
			{
				//Printing which players turn it is and which player they are in out of the remaining alive players
				printf("\nPLAYER %d - %d OF %d\n\n",i+1, playerRound, n-deadPlayers);
				if(deadPlayers>0){
					printf("%d player(s) died/left!\n\n",deadPlayers);
				}

				playerAttackCheck(board, player, &checkerD, i, playersFound);	//Setting attack player's capabilities


				do{
					if(((player[i].magicCheck!=1)&&(player[i].distantCheck!=1)&&(player[i].nearCheck!=1)))
					{
						printf("Player %d, no other players in range!\n", i+1);
					}
					//Asking user for input on what action they would like to take
					printf("Would you like to:\n");
					//Only the options available to user are printed
					if(player[i].magicCheck==1)
					{
						puts("4 - Attack a player with a magic attack");
					}
					if(player[i].distantCheck==1)
					{
						puts("3 - Attack a distant player");
					}
					if(player[i].nearCheck==1)
					{
						puts("2 - Attack a nearby player");
					}
					printf("1 - Move to an adjacent slot\n");
					printf("0 - Quit the game\n");
					fflush(stdin);
					scanf("%d", &choice);

					allowedChoice = 0;	//Resetting allowedChoice to 0 as the default
					//Checking input if allowed for attacks.
					switch(choice)
					{
						case 2:
							if(player[i].nearCheck==1)
							{
								allowedChoice = 1;
							}
							else
							{
								allowedChoice = 0;
								puts("\nIncorrect number entered!\n");
							}
							break;
						case 3:
							if(player[i].distantCheck==1)
							{
								allowedChoice = 1;
							}
							else
							{
								allowedChoice = 0;
								puts("\nIncorrect number entered!\n");
							}
							break;
						case 4:
							if(player[i].magicCheck==1)
							{
								allowedChoice = 1;
							}
							else
							{
								allowedChoice = 0;
								puts("\nIncorrect number entered!\n");
							}
							break;
					}
				}while(choice != 1 && choice != 0 && allowedChoice !=1 );	//Continues unless move, quit or an allowed attack option is selected

				//Carries out chosen action
				switch(choice)
				{
					case 4:	//Magic attack
						//Loop to print all players alive since any alive player can be attacked
						for(j = 0;j<n;j++)
						{
							if(player[j].lifepoints>0 && j != i)
							{
								printf("%s can be attacked. ID = %d\n",player[j].name, j);
							}
						}
						//Asks player to input a player id to attack until a valid choice is made
						do{
							printf("Enter the ID number of the player you wish to attack: ");
							scanf("%d", &choice);
						}while(choice == i || player[choice].lifepoints<=0 || choice>=n || choice<0);

						//Alert of attack
						printf("\n\n**Attacking %s(%d) with a magic attack.**\n\n",player[choice].name,choice);
						//Calling magicAttack
						magicAttack( &player[i], &player[choice], board); //first arg is attacker, second is attacked
						break;

					case 3:	//Distant attack
						//Resetting playersFound array to 0
						for(j=0;j<PLAYER_MAX;j++)
						{
							playersFound[j]=0;
						}
						//Calling findSlots for distance 2, 3 & 4 to get all attackable players of distant attack
						for(j=2;j<5;j++)
						{
							findSlots(j, 0,  &board[player[i].row][player[i].column], playersFound, &checkerD, i);
						}
						//Printing all attackable players with distant attack.
						for(j=0;j<n;j++)
						{
							if(playersFound[j]==1)
							{
								printf("%s can be attacked. ID = %d\n",player[j].name, j);
							}
						}
						//Asks player to input a player id to attack until a valid choice is made
						do{
							printf("Enter the ID number of the player you wish to attack: ");
							scanf("%d", &choice);
						}while(choice>n || choice<0 || playersFound[choice]!=1);
						//Alert of attack
						printf("\n\n**Attacking %s(%d) with distant attack.**\n\n",player[choice].name,choice);
						//Call distantAttack
						distantAttack( &player[i], &player[choice], board);

						break;

					case 2:	//Near attack
						//Resetting playersFound array to 0
						for(j=0;j<PLAYER_MAX;j++)
						{
							playersFound[j]=0;
						}
						//Finding all attackable players with near attack
						checkNearAttack(board, player[i].row, player[i].column, i, playersFound);
						//Printing all attackable players with near attack
						for(j=0;j<n;j++)
						{
							if(playersFound[j]==1)
							{
								printf("%s can be attacked. ID = %d\n",player[j].name, j);
							}
						}
						//Asks player to input a player id to attack until a valid choice is made
						do{
							printf("Enter the ID number of the player you wish to attack: ");
							scanf("%d", &choice);
						}while(choice>n || choice<0 || playersFound[choice]!=1);
						//Alert of attack
						printf("\n\n**Attacking %s (ID: %d) with near attack.**\n\n",player[choice].name,choice);
						//Calling near attack
						nearAttack( &player[i], &player[choice], board);

						break;

					case 1:	//move slot
						playerMoveChoice(board, player, &slotChoice, n, i);
						break;
					case 0:	//quit
						playerQuit(board, &player[i]);
						break;
				}
				if(n-1<=deadPlayers)	//If 1 player remains, break;
				{
					break;
				}
			}
			else
			{
				playerRound--; //Decrementing playerRound number so that correct number is displayed
			}
		}
		roundNum++;	//Increment round number.
	}while(n-1>deadPlayers);	//Continue until 1 player or less remains

	if(n>deadPlayers)
	{
		for(i=0;i<n;i++)
		{
			if(player[i].lifepoints>0)
			{
				printf("\n%s has won the game! GG\n\n",player[i].name);
				break;
			}
		}
	}
	else
	{
		puts("\nIt appears all players are dead. GG\n\n");
	}
}
