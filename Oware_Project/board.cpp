#include "oware.h"
#include <iomanip>

extern Player a; extern Player b;

/**
@return proper console width if windows/linux; else minimum recommended
*/
int consoleWidth() {

#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	return columns;
#endif

#ifdef __linux__
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	return size.ws_col;
#endif

	return REC_CONSOLE_WIDTH;
}

/** Prepares new message for a player (to be displayed next turn).
If less than 5 captures made, the message will be a tip/instruction.
Else, it will be a provocation/info according to the score of the game.
@param player - player whose message attribute will be changed.
*/
void generateMessage(Player p) { //is called at the end of doCaptures
	int i = rand() % 3;

	// less than 4 captures - show tips even in computer message
	if ((a.computer && b.score < 4) || (!a.computer && p.score < 4)) {
		switch (i) {
		case 0:
			if (p.charName == 'A') a.message = "Capture 25 seeds to win.";
			if (p.charName == 'B') b.message = "Sow one seed per house.";
			break;
		case 1:
			if (p.charName == 'A') a.message = "Sowing occurs counterclockwise.";
			if (p.charName == 'B') b.message = "Capturing occurs clockwise after sowing.";
			break;
		case 2:
			if (p.charName == 'A') a.message = "Do not leave your opponent without moves.";
			if (p.charName == 'B') b.message = "Capture consecutive houses with 2 or 3 seeds.";
			break;
		}
	}

	//computer has no message
	else if (p.charName == 'A' && a.computer) a.message = " ";

	else if (p.charName == 'A' && !a.computer) {
		if (a.score > 4 * LINES) a.message = "Only " + to_string(25 - a.score) + " points to go!"; //final incentive message
		else {
			switch (i) {
			case 0:
				if (a.score > b.score) a.message = "You're leading with " + to_string(a.score) + " seeds captured.";
				else if (a.score == b.score) a.message = "It seems to be equal.";
				else a.message = "Are you " + to_string(b.score - a.score) + " times worse than " + b.name + "?";
				break;
			case 1:
				if (a.score > 5) a.message = to_string(25 - a.score) + " points separate you from victory.";
				else a.message = "Don't think " + to_string(a.score) + " points will lead you anywhere.";
				break;
			case 2:
				if (b.score - a.score > 2) a.message = "In my time a " + to_string(b.score - a.score) + " points gap was humiliating.";
				else a.message = "Is it me or are your hands shaking?";
				break;
			}
		}
	}
	else if (p.charName == 'B') {
		if (b.score > 4 * LINES) b.message = "Only " + to_string(25 - b.score) + " points to go!"; //final incentive message
		else {
			switch (i) {
			case 0:
				if (a.score > b.score) b.message = to_string(a.score - b.score) + " points down is pretty huge...";
				else if (a.score == b.score) b.message = "Are you and " + a.name + " close?";
				else b.message = "I've seen plenty lose " + to_string(b.score - a.score) + " points advantages.";
				break;
			case 1:
				if (b.score > 5) b.message = "Who thought " + to_string(25 - b.score) + " points were so hard?";
				else b.message = "You know you win at 25, right?";
				break;
			case 2:
				if (a.score - b.score > 2) b.message = "Maybe ask for some help?";
				else b.message = "I can feel the tension in your eyes.";
				break;
			}
		}
	}
}

/** Prints a colored messaged using ANSI escape codes.
No lines before or after.
WILL NOT WORK PROPERLY on Windows 8.1- or ancient Linux versions.
*/
void print(string message, string color) {
	cout << "\x1B[" << color << "m" << message << "\033[0m\t\t";
}

/** Helper to printHouses to design the interface inside the houses.
@param remaining - seeds yet to print inside a house.
@param column - column printHouses is writing in.
@param total - total seeds to print in the house.
@return SEED or SPACE according to the situation.
*/
char seedToPrint(int remaining, int column, int total) {

	if (remaining > 0) { // there are still seeds to print

		if (total > ((WIDTH - 1) * LINES / 2)) { // 3 columns aren't enough

			if (total > (WIDTH - 3)* LINES) return SEED; // 7 columns

			else { // 4 columns
				if (column % 2 == 0) return SEED;
				else return SPACE;
			}
		}

		else if (column != 0 && column != WIDTH - 1) { // 3 columns
			if (column % 2 != 0) {
				//initial board positioning design refinement
				if (column == WIDTH - 2 && total == 4) return SPACE;
				else return SEED;
			}
			else return SPACE;
		}
		else return SPACE;
	}
	else return SPACE;
}

/** Print the houses of a player and displays messages if function.
Calls seedToPrint() to help with spacing inside the houses.
@param player - player whose houses will be printed.
@param scoreSquare - place where the score house will be (must be "right" or "left")
@param function - if TRUE, a message will be displayed (active turn)
*/
void printHouses(Player p, string scoreSquare, bool function) {
	int housesToPrint[6]; int scoreToPrint = p.score; char seed;

	for (int i = 0;i < 6;i++) {
		housesToPrint[i] = p.house[i];
	}

	for (int line = 0;line < LINES;line++) {
		for (int square = -1;square < 7;square++) {
			cout << VERTICAL;
			for (int i = 0;i < WIDTH;i++) {
				if (square == -1) {
					//score square (left)
					if (scoreSquare != "left") {
						cout << SPACE; //empty house
						continue;
					}
					seed = seedToPrint(scoreToPrint, i, p.score);
					cout << seed;
					if (seed == SEED) scoreToPrint -= 1;
				}
				else if (square == 6) {
					//score square (right)
					if (scoreSquare != "right") {
						cout << SPACE;
						continue;
					}
					seed = seedToPrint(scoreToPrint, i, p.score);
					cout << seed;
					if (seed == SEED) scoreToPrint -= 1;
				}
				else {
					//houses
					seed = seedToPrint(housesToPrint[square], i, p.house[square]);
					cout << seed;
					if (seed == SEED) housesToPrint[square] -= 1;
				}
			}
		}
		cout << VERTICAL; // separator before messages

		if (line == 1) {
			if (p.charName == 'A') {
				if (a.turn && function) {
					if (!a.computer) print(" " + a.name + " - your turn!", a.color);
					else print(" " + a.name + " to play...", a.color);  // computer doesn't need enthusiasm
				}
				else if (!function) cout << " " + a.name;
			}
			else {
				if (b.turn && function) print(" " + b.name + " - your turn!", b.color);
				else if (!function) cout << " " + b.name;
			}
		}
		else if (line == 2 && function) { // when not function, don't show special messages
			if (p.charName == 'A' && a.turn) cout << " " << a.message;
			else if (p.charName == 'B' && b.turn) cout << " " << b.message;
		}

		cout << "\n";
	}
}

/** Prints the line with house numbers (0-5).
*/
void printHouseNumbers() {
	//score 1 spacing
	for (int i = 0;i < WIDTH + 2;i++) {
		cout << SPACE;
	}

	//houses
	for (int square = 0;square < 6;square++) {
		cout << right << setw(WIDTH / 2 + 1) << setfill(SPACE) << square;
		for (int j = 0;j < WIDTH / 2 + 1;j++) {
			cout << SPACE;
		}
	}

	cout << "\n";
}

/** Prints a separator line.
@param sep - char to be printed across the line.
@param form - if "divided" will write across the score houses too, else won't.
*/
void printSeparatorLine(char sep, string form) {
	//score 1
	for (int i = 0;i < WIDTH + 1;i++) {
		if (i == 0 && form == "divided") cout << VERTICAL;
		else {
			if (form == "divided") cout << SPACE;
			else cout << sep;
		}
	}

	//houses
	for (int squares = 0;squares < 6;squares++) {
		if (form == "divided") cout << VERTICAL;
		else cout << sep;

		for (int i = 0;i < WIDTH;i++) {
			cout << sep;
		}
	}

	//score 2
	for (int i = 0;i < WIDTH + 1;i++) {
		if (i == 0 && form == "divided") cout << VERTICAL;
		else {
			if (form == "divided") cout << SPACE;
			else cout << sep;
		}
	}

	if (form == "divided") cout << VERTICAL << "\n";
	else cout << sep << "\n";
}

/** Scrolls just enough that previous information is not seen.
Works with a ANSI escape code.
WILL NOT WORK PROPERLY on Windows 8.1- or ancient Linux versions.
*/
void clearScreen()
{
	cout << "\x1B[2J\x1B[H"; //this requires support for ansi escape codes. otherwise will not work
}

/** Calls helper functions to draw the board.
@param function - will be passed to the helpers. if TRUE, messages will be shown, else won't.
*/
void printBoard(bool function) {

	while (consoleWidth() < REC_CONSOLE_WIDTH) { //messages must fit in screen
		cout << "\n"; print("OWARE GAME", YELLOW); cout << "\n\n";
		cout << "ERROR! Please expand the console to avoid formatting issues!";
		waitEnter();
		clearScreen();
	}

	if (eligibleDraw()) function = false;

	cout << "\n";
	if (function) print("OWARE GAME", YELLOW);
	else cout << "OWARE GAME";
	cout << "\n\n";

	if (a.turn && function) printHouseNumbers();
	else cout << "\n";

	printSeparatorLine(HORIZONTAL2, "full");
	printHouses(a, "left", function);
	printSeparatorLine(HORIZONTAL2, "divided");
	printHouses(b, "right", function);
	printSeparatorLine(HORIZONTAL2, "full");

	if (b.turn && function) printHouseNumbers();
	else cout << "\n";

	cout << "\n";
}