/*
 * crossfireOperations.h
 *
 *  Created on: 9 Apr 2017
 *      Author: Aidan
 */

#ifndef CROSSFIREOPERATIONS_H_
#define CROSSFIREOPERATIONS_H_



#endif /* CROSSFIREOPERATIONS_H_ */

#define BOARD_SIZE 7
#define REQ_DISTANCE 3
#define PLAYER_MAX 6

enum playerType {humanType, ogreType, wizardType, elfType};
enum slotType {level_ground, cityType, hillType};
/*
 * Definition of boolean types
 * This avoids using <stdbool.h>
 */
typedef int bool;
enum { false, true };

/*
 * The slot template
 */
struct slot{
	bool explored;
	//row number
	int row;
	//column number
	int column;
	//Type of slot
	char slot_name[15];
	int slot_type;
	//Represents if there is at least one player in the slot
	int occupied;
	//Represents which players are in the slot
	int playersPresent[PLAYER_MAX];
	// adjacent left slot
	struct slot *left;
	// adjacent right slot
	struct slot *right;
	// adjacent up slot
	struct slot *up;
	// adjacent down slot
	struct slot *down;
}slot;

struct player_type{ // Player template
	char name[20];
	char type[20];
	float lifepoints;
	int smartness;
	int strength;
	int magicskills;
	int luck;
	int dexterity;
	int row;
	int column;
	int id;
	int nearCheck, distantCheck, magicCheck;
};

void createBoard(struct slot **board, int boardSize); // Creates the board

void findSlots(int reqDist, int currDist,  struct slot * currSlot, int * playersFound, int *checker, int x); // Finds players a certain distance away for attacks

int checkSlot(int row, int column, struct slot ** board, int x, int * playersFound); // Checks if a slot is occupied by anyone that is not the player

int checkNearAttack(struct slot ** board, int row, int column, int x, int * playersFound); // Check if a near attack is possible

void printPlayersStats(struct player_type *player, int n, struct slot ** board); // Print player stats

void playertypeHuman(struct player_type *player, struct slot **board); // Assign stats based on player type

void playertypeOgre(struct player_type *player, struct slot **board);

void playertypeWizard(struct player_type *player, struct slot **board);

void playertypeElf(struct player_type *player, struct slot **board);

void nearAttack(struct player_type *attacker, struct player_type *attacked, struct slot ** board); // Attacks based on different types

void distantAttack(struct player_type *attacker, struct player_type *attacked, struct slot ** board);

void magicAttack(struct player_type *attacker, struct player_type *attacked, struct slot ** board);

void playeronCity(struct player_type * player); // Change player stats if they move onto a City

void playeronHill(struct player_type * player); // Change player stats if they move onto a Hill

void playerMoveChoice(struct slot ** board, struct player_type * player, char *slotChoice, int n, int i); // See if players can move or not

void MoveSlot(struct slot ** board,struct player_type * player, const int numOfPlayers, int row, int column, int i); // Change position of player if they decide to move

void playerQuit(struct slot ** board,struct player_type * player); // Remove player from board and stats if they die or decide to quit

void playerAttackCheck(struct slot ** board, struct player_type * player, int * checkerD, int x, int * playersFound); // Check possibilities for attack types

//Function to initialize players
void playerInit(struct slot ** board, struct player_type * player,int i);

//Function which executes the rounds of the game until 1 player remains
void playcrossFire(struct slot **board, struct player_type * player, int n, int deadPlayers);
