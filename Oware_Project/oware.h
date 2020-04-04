#pragma once

/*
PROJECT SPECS
Oware Game - Abapa Version
Programming, MIEIC - 19/20, FEUP
By Bruno Daniel Mendes
up201906166

SPECIAL RULES
Original rules from the Abapa version include:
- If possible, give the opponent seeds when he hasn't any
- Capture own pieces if opponent has no seeds and can't give him any
- Skip original house when 12+ seeds
- Grand Slams are allowed, but no capture is made
Rules added by me:
- Draw proposal after 25 moves if both players have 5 seeds or less
- Forced draw after 50 moves if both players have 10 seeds or less

BOT DETAILS
- Hard mode: checks for his and opponent's captures. If his are higher or equal,
captures; else prevents captures. If none of these apply, will randomize.
- Easy mode: makes a random possible move.

KNOWN ISSUES
- No real time board update during movement: this was tested
(see commented line under move()) with problems (acceptance of keyboard
input while asleep; console flickering on Windows).
*/

#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
//used by consolewidth()
#include <sys/ioctl.h> 
#include <unistd.h>
//replace windows function
#define Sleep(x) usleep((x)*1000)
#endif

using namespace std;

//design choices
const int NAME_LENGTH_LIMIT = 20;
const char VERTICAL = '|'; const char HORIZONTAL1 = '_';  const char HORIZONTAL2 = '-'; const char SPACE = ' '; const char SEED = 'o';
const int WIDTH = 7; const int LINES = 4; //width must be odd>=7 and lines must be >= 4
const int REC_CONSOLE_WIDTH = 120; //recommended width to allow tips to be displayed at the side

//colors
const string RED = "31"; const string GREEN = "32";
const string BLUE = "36"; const string YELLOW = "33";

//draw flags
const int DRAW_PROPOSAL = 25; //moves before asking for draw if conditions met
const int M_DRAW_SEEDS = 5; //max seeds to allow draw
const int MOVE_LIMIT = 2 * DRAW_PROPOSAL; //each player has 50 moves to try to win. else forced draw (design choice)

/*
The attributes of a player that will be used across the entire program.
About the constructor:
@param setting - if "first", will play first, else won't.
@param color - the color that will be be shown in this player's messages.
@param letter - the player identifier ('A' or 'B) to be used by functions
*/
class Player {
public:
	string color; // to be displayed at the right of the board
	string name;
	bool computer;
	bool computer_hard; //calculate instead of randomize
	char charName;
	int score;
	int nSeeds; //if nSeeds = 0, special constraints are applied to the opponent
	int house[6];
	bool turn;
	bool win;
	bool side; // tells if current path in his size
	bool mayRemoveAllPieces;
	bool wantsDraw;
	int nMoves;
	string message;

	Player(string setting, string name_color, char letter) { //constructor

		if (setting == "first") {
			side = true;
			name = "Player A";
		}

		else {
			side = false;
			name = "Player B";
		}

		//distribute seeds
		for (int i = 0; i < 6; i++) {
			house[i] = 4;
		}

		turn = side;
		computer = false; //may change after intro
		computer_hard = false;
		color = name_color;
		charName = letter;
		message = "We'll help you with tips as you go.";
		mayRemoveAllPieces = false;
		score = 0;
		nSeeds = 24;
		win = false;
		wantsDraw = true; //allow draw message to be shown once
		nMoves = 0;
	}

	/** Sets turn true.
	*/
	void goFirst() {
		side = true;
		turn = side;
	}

	/** Sets turn false.
	*/
	void goLast() { //sets turn false
		side = false;
		turn = side;
	}
};

int consoleWidth();
int enableVTProcessing();
int eligibleDraw();
void waitEnter();
string formatName(string name);
int askTwoOptions(string message);
int maxElementIndex(vector<int> v);
int bestMove(Player p, bool PreventCaptures = true);
int calculateScore(Player p, int initial);
bool hasCaptures(Player p);
int calculateFinal(Player p, int initial);
void printBoard(bool function = true);
void play();
void move(int initial);
void doCaptures(int initial);
void switchSides();
void switchTurns();
bool hasWinner();
void updatenSeeds();
bool inBounds(int house);
bool possibleMove(Player p, int initial, bool output = true);
bool cannibalize(bool remove = false);
void clearScreen();
char seedToPrint(int remaining, int column, int total);
void print(string message, string color);
void printHouses(Player p, string scoreSquare, bool function);
void printHouseNumbers();
void printSeparatorLine(char sep, string form);
void generateMessage(Player p);
void endgame();
void intro();
string askName(string message);
void abortGame();
bool proposeDraw();
bool grandSlam(int final);