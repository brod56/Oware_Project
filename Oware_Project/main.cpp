#include "oware.h"
#include <ctime>

Player a("first", BLUE, 'A'); Player b("last", GREEN, 'B');

/** Calls intro and then loops while no player has won. Finally, jumps to endgame.
*/
int main()
{
	srand((unsigned int) time(NULL)); // for random messages
	cin.sync_with_stdio(false); // experiment to improve performance
	enableVTProcessing(); // color codes, console width etc

	intro();
	clearScreen();

	do {
		printBoard();
		play();
		switchTurns();
		clearScreen();
	} while (!hasWinner());

	clearScreen();
	endgame();
}

/** Enables support for ANSI escape codes in Windows 10.
This is a function taken from the MSDN documentation.
See details there.
*/
int enableVTProcessing(){
#ifdef _WIN32
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return GetLastError();
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return GetLastError();
	}

	return 0;
#endif
}

/** The welcome page to the game before it starts.
Asks if user wants 1v1 or 1vbot.
Changes players' attributes accordingly.
*/
void intro() {
	printBoard(false);
	int ans1, ans2;
	string a_name, b_name;

	print("WELCOME!", YELLOW); cout << "\n\n";

	//computer or 1v1
	ans1 = askTwoOptions("Choose a play mode:\n\n1. One player\n2. Two players\n\n");

	if (ans1 == 1) {
		a.computer = 1;
		a.color = RED;

		clearScreen(); printBoard(false);
		int ans3 = askTwoOptions("Choose bot:\n\n1. Easy - a nice guy to hang with\n2. Hard - a relentless killing machine\n\n");

		if (ans3 == 2) {
			a.computer_hard = true;
			a.name = "Hard Bot";
		}
		else
		{
			a.computer_hard = false;
			a.name = "Easy Bot";
		}
	}


	//player names
	clearScreen(); printBoard(false);
	cout << "I'd like to know you a bit better...\n" << "\n";

	if (!a.computer) {
		for (;;) { //breaks if different names
			a_name = askName("Player A's name: ");
			cout << "\n";
			b_name = askName("Player B's name: ");
			if (a_name != b_name) break;
			else cout << "\n" << "Player names cannot be the same. Please try again.\n";
		}
		a.name = a_name; b.name = b_name; cout << "\n";
	}
	else {
		b_name = askName("What's your name? ");
		b.name = b_name; cout << "\n";
	}

	//first to play
	clearScreen(); printBoard(false);
	ans2 = askTwoOptions("Who should go first?\n\n1. " + a.name + "\n" + "2. " + b.name + "\n\n");

	if (ans2 == 1) {
		a.goFirst(); b.goLast();
	}
	else {
		a.goLast(); b.goFirst();
	}

	//get to the game
	clearScreen(); printBoard(false);
	print("GET READY!", YELLOW);
	cout << "\n\n" << "An epic battle is about to start.\n" << "\n" << "Press enter to start the game...\n";
	cin.ignore(10000, '\n');
}

/** Displays winner of the game.
Gets info from player.win attribute.
*/
void endgame() {
	printBoard(false);

	if (a.win && !b.win && a.computer) {
		print("THE GAME HAS ENDED!", a.color);
		cout << "\n\n" << "The computer has devastated you!\n";
		cout << "A walk in the park: " << a.score << " points.\n" << "\n";
	}

	else if (a.win && !b.win) {
		print("THE GAME HAS ENDED!", a.color);
		cout << "\n\n" << "Congratulations, " << a.name << "!\n";
		cout << "You won with a score of " << a.score << " points.\n" << "\n";
	}

	else if (b.win && !a.win) {
		print("THE GAME HAS ENDED!", b.color);
		cout << "\n\n" << "Congratulations, " << b.name << "!\n";
		cout << "You won with a score of " << b.score << " points.\n" << "\n";
	}

	else {
		print("THE GAME HAS ENDED!", YELLOW);
		cout << "\n\n" << "It's a draw.\n";
		cout << "Both players scored 24 points.\n" << "\n";
	}

	cout << "Press enter to exit...\n";
	cin.ignore(10000, '\n');
}