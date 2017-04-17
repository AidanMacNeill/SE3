/*
 * boardCreation.c

 *
 *  Created on: 9 Apr 2017
 *      Author: Aidan
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crossfireOperations.h"

void createBoard(struct slot **board, int boardSize)
{
	for(int i =0; i< boardSize; i++)
	{
		//For each slot it sets up the row and column number
		for(int j=0;j < boardSize; j++)
		{
			board[i][j].row = i;
			board[i][j].column = j;
			//Assigning random slot type
			int x = rand() % 3;
			board[i][j].slot_type = x;
			//Copying name of slot to struct member slot_name
			if(x == 0)
			{
				strcpy(board[i][j].slot_name,"Level Ground");
			}
			else if(x == 1)
			{
				strcpy(board[i][j].slot_name, "City");
			}
			else if(x == 2)
			{
				strcpy(board[i][j].slot_name, "Hill");
			}
	}

	for(int i = 1; i< boardSize-1; i++) // Set up adjacent slots in all 4 directions
	{
		for(int j=1;j < boardSize-1; j++)
		{
			board[i][j].up = &board[i-1][j];
			board[i][j].right = &board[i][j+1];
			board[i][j].down = &board[i+1][j];
			board[i][j].left = &board[i][j-1];
		}
	}

	for(int j = 1; j < boardSize -1; j++)
	{
		board[0][j].right = &board[0][j+1]; // Set up slots on top row of board
		board[0][j].left = &board[0][j-1];
		board[0][j].down = &board[1][j];
		board[0][j].up = NULL;

		board[boardSize - 1][j].right = &board[boardSize - 1][j+1]; // Set up slots on bottom row of board
		board[boardSize - 1][j].left = &board[boardSize - 1][j-1];
		board[boardSize - 1][j].up = &board[boardSize - 2][j];
		board[boardSize - 1][j].down = NULL;
	}

	for(int i = 1; i < boardSize -1; i++)
	{
		board[i][0].right = &board[i][1]; // Set up slots on leftmost column on board
		board[i][0].up = &board[i-1][0];
		board[i][0].down = &board[i+1][0];
		board[i][0].left = NULL;

		board[i][boardSize-1].left = &board[i][boardSize-2]; // Set up slots on rightmost column on board
		board[i][boardSize -1].up = &board[i -1][boardSize-1];
		board[i][boardSize -1].down = &board[i+1][boardSize -1];
		board[i][boardSize -1].right = NULL;
	}

		board[0][0].right = &board[0][1]; // Set up slots at (0,0)
		board[0][0].down = &board[1][0];

		board[0][boardSize-1].left = &board[0][boardSize-2]; // Set up slots at top right of board
		board[0][boardSize -1].down = &board[1][boardSize -1];

		board[boardSize -1][0].right = &board[boardSize -1][1]; // Set up slots at bottom left of board
		board[boardSize -1][0].up = &board[boardSize -2][0];

		board[boardSize - 1][boardSize-1].up = &board[boardSize-2][boardSize-1]; // Set up slots at bottom right of board
		board[boardSize - 1][boardSize -1].left = &board[boardSize -1][boardSize -2];
	}
}

void findSlots(int reqDist, int currDist,  struct slot * currSlot, int * playersFound, int *checker, int x)
{
	//The base case: the current slot is at the required distance from the starting slot
	if(currDist == reqDist)
	{
		//the current slot was not explored
		if(currSlot -> explored == false)
		{
			//The current slot is occupied
			if(currSlot -> occupied == true){
				// Search through array playersPresent for true indicating player i is occupying current slot
				for(int i =0; i<PLAYER_MAX;i++)
				{
					if(currSlot -> playersPresent[i] == 1 && i != x && playersFound[i] != 1){
						playersFound[i] = 1;	// Set playerFound to true so for player i
						(*checker)++;		//increment checkerD
					}
				}
			}
		}
	}
	//The recursive call: the current slot is at a distance that is less than the required distance (more slots still have to be traversed)
	else{
		//if the current slot has the up slot != NULL (i.e. the slot is not in the first row)
		if(currSlot->up != NULL)
		{
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the up slot
			findSlots(reqDist, currDist +1,  currSlot->up, playersFound, checker, x);
		}
		//if the current slot has the right slot != NULL (i.e. the slot is not in the last column)
		if(currSlot->right != NULL)
		{
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the right slot
			findSlots(reqDist, currDist +1,  currSlot->right, playersFound, checker, x);
		}
		//if the current slot has the down slot != NULL (i.e. the slot is not in the last row)
		if(currSlot->down != NULL)
		{
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the down slot
			findSlots(reqDist, currDist +1,  currSlot->down, playersFound, checker, x);
		}
		//if the current slot has the left slot != NULL (i.e. the slot is not in the first column)
		if(currSlot->left != NULL)
		{
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the left slot
			findSlots(reqDist, currDist +1,  currSlot->left, playersFound, checker, x);
		}
	}
}

int checkNearAttack(struct slot ** board, int row, int column, int x, int * playersFound)
{
	bool check = false; // Initialise value to false

	if((checkSlot(row,column,board, x, playersFound)) == true) // Check for occupant in same slot
	{
		check = true;
	}

	if(row+1 < BOARD_SIZE)
	{
		if((checkSlot(row+1,column,board, x, playersFound)) == true) // Check for occupant above
		{
			check = true;
		}
	}

	if(column+1 < BOARD_SIZE)
	{
		if((checkSlot(row,column+1,board, x, playersFound)) == true) // Check for occupant to the right
		{
			check = true;
		}
	}

	if(row-1 >= 0)
	{
		if((checkSlot(row-1,column,board, x, playersFound)) == true) // Check for occupant below
		{
			check = true;
		}
	}

	if(column-1 >= 0)
	{
		if((checkSlot(row,column-1,board, x, playersFound)) == true) // Check for occupant to the left
		{
			check = true;
		}
	}
	return check;
}

int checkSlot(int row, int column, struct slot ** board, int x, int * playersFound)
{
	if(board[row][column].occupied)
	{
		for(int i = 0; i < PLAYER_MAX; i++)
		{
			if(board[row][column].playersPresent[i] == 1 && i != x) // Check if a player other than current one is in slot
			{
				playersFound[i] = 1;	// set playerFound[i] so player can be chosen
				return true;
			}
		}
	}
	return false;
}

void playeronCity(struct player_type * player) // Change stats if player is on a city as described in the brief
{
	if(player->smartness > 60)
	{
		if(player->magicskills >= 90)
		{
			player->magicskills = 100;
		}
		else if(player->magicskills < 90)
		{
			player->magicskills += 10;
		}
	}
	else if(player->smartness <= 50)
	{
		if(player->dexterity <= 10)
		{
			player->dexterity = 0;
		}
		else if(player->dexterity > 10)
		{
			player->dexterity -= 10;
		}
	}
}

void playeronHill(struct player_type * player) // Change stats if player is on a Hill as described in the brief
{
	if(player->dexterity >= 60)
	{
		if(player->strength >= 90)
		{
			player->strength = 100;
		}
		else if(player->strength < 90)
		{
			player->strength += 10;
		}
	}
	else if(player->dexterity < 50)
	{
		if(player->strength <= 10)
		{
			player->strength = 0;
		}
		else if(player->strength > 10)
		{
			player->strength -= 10;
		}
	}
}
