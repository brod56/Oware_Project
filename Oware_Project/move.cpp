#include "oware.h"

extern Player a; extern Player b;

/** Show draw offers/forced draw warnings if conditions are met.
Else display computer's chosen move or ask the player to choose.
Ensures the move is possible and then calls move() to do its magic.
After this, if no possible moves in the next turn, calls cannibalize().
*/
void play() {

	int initial;

	//draw offer/forced draw
	int endgame = eligibleDraw();
	if (endgame == 1) {
		if (proposeDraw()) return;
	}
	else if (endgame == 2) {
		abortGame();
		return; //prevent asking for move
	}

	if (a.turn) {
		if (!a.computer) cout << a.name << ", make a move: ";
		else {
			if (a.computer_hard) {
				initial = bestMove(a); //if computer hard, calculate initial
			}
			else {
				do { // do not attempt to move from empty house
					initial = rand() % 6;
				} while (!possibleMove(a, initial, false));
			}
			cout << "The computer will play from house " << initial << ".";
			waitEnter();
		}
		a.nMoves++;
	}
	else {
		cout << b.name << ", make a move: ";
		b.nMoves++;
	}

	if (!(a.turn && a.computer)) { //ask for keyboard move input
		while (!(cin >> initial) || (a.turn && !possibleMove(a, initial)) || (b.turn && !possibleMove(b, initial))) {
			//possiblemove will present a error message
			if (cin.fail()) {
				cin.clear();
				cout << "\n" << "Please pick seeds from one of your houses (0-5).\n";
			}
			cout << "Enter a proper move: ";
			cin.ignore(10000, '\n');
		}
		cin.ignore(10000, '\n');
	}

	move(initial);

	if (cannibalize()) { //after possiblemove() is called, cannibalize may verify if own pieces should be captured
		switchTurns(); clearScreen(); printBoard(false); //remove messages
		cannibalize(true); //remove all seeds
		print("NO POSSIBLES MOVES LEFT!", YELLOW); cout << "\n";
		if (a.turn) cout << "\n" << a.name << " cannot move. As a result, " << b.name << " may capture his own seeds.\n";
		if (b.turn) cout << "\n" << b.name << " cannot move. As a result, " << a.name << " may capture his own seeds.\n";
		waitEnter();
	}
}

/** Goes counterclokwise around the board seeding 1 seed per house until the player has no seeds in hand.
Should skip original house if it had 12+ seeds.
After the move is complete call doCaptures() to consume seeds if possible and update attributes.
@param initial - the house to pick seeds from.
*/
void move(int initial) {
	int final; int current = initial; int max_moves;

	if (b.turn) {
		if (a.side) switchSides(); max_moves = b.house[initial]; b.house[initial] = 0;
	}

	else {
		if (b.side) switchSides(); max_moves = a.house[initial]; a.house[initial] = 0;
	}

	for (int moves = 0; moves < max_moves; moves++) {
		if (b.side) {
			if (!inBounds(current + 1)) {
				switchSides(); //when switching sides, house number doesnt change
				if (!(a.turn && current == initial)) a.house[current] += 1; //must skip original house
				else { moves--; continue; } //jumps, doesnt count as move
			}
			else {
				current += 1;
				if (!(b.turn && current == initial)) b.house[current] += 1;
				else { moves--; continue; }
			}
		}
		else if (a.side) {
			if (!inBounds(current - 1)) {
				switchSides();
				if (!(b.turn && current == initial)) b.house[current] += 1;
				else { moves--; continue; }
			}
			else {
				current -= 1;
				if (!(a.turn && current == initial)) a.house[current] += 1;
				else { moves--; continue; }
			}
		}
		/*-------------------------------------------
		//this line enables board updating during movements
		//check header file for discussion about this
		clearScreen(); printBoard(false); Sleep(500);
		---------------------------------------------*/
	}
	updatenSeeds();
	final = current;
	doCaptures(final); //final will be docaptures() start
}

/** Check for captures clockwise and consumes seeds if that's the case.
Captures should happen in consecutive opponent houses with 2 or 3 seeds.
At the end, calls generatemessage() if no captures, else changes the message itself.
If captured, updates scores.
@param initial - the house to be calculated from (final sowed house).
*/
void doCaptures(int initial) {

	int current = initial; int captured = 0;

	if (!grandSlam(initial)) { // do not capture if all seeds would be captured
		for (int i = 0; i < 5; i++) { //breaks when cannot capture anymore or 5 houses are captured
			updatenSeeds();	//one may not capture all opponent's pieces
			if (a.turn && b.side) {
				if (b.house[current] == 2 || b.house[current] == 3) { //abapa rule
					captured += b.house[current]; a.score += b.house[current];
					b.house[current] = 0; //empty captured house
					if (!inBounds(current - 1)) break; //if next house is in players side
					else current -= 1;
				}
				else break; //stop when consecutive rule is broken
			}
			else if (b.turn && a.side) {
				if (a.house[current] == 2 || a.house[current] == 3) {
					captured += a.house[current]; b.score += a.house[current];
					a.house[current] = 0;
					if (!inBounds(current + 1)) break;
					else current += 1;
				}
				else break;
			}
			else break; //not in opponent's side
		}
	}

	updatenSeeds(); //necessary for draw/endgame offers check next turn

	if (captured) {
		if (a.turn) {
			b.message = "Careful! " + a.name + " just captured " + to_string(captured) + " seeds.";
			generateMessage(a);
		}
		else if (b.turn) {
			if (!a.computer) a.message = "Careful! " + b.name + " just captured " + to_string(captured) + " seeds.";
			else a.message = "The machine is not happy! You captured " + to_string(captured) + " seeds.";
			generateMessage(b);
		}
	}
	else { //preparation for next turn messages
		generateMessage(a); generateMessage(b);
	}
}

/** Checks if a Grand Slam is possible in the position after sowing.
@param final - last sowed house after moving.
@return TRUE if possible Grand Slam, else FALSE.
*/
bool grandSlam(int final) {
	int captured = 0;
	if (a.turn && b.side) {
		while (final >= 0) {
			if (b.house[final] != 2 && b.house[final] != 3) break;
			captured += b.house[final]; final--;
		}
		if (captured == b.nSeeds) return true;
	}
	else if (b.turn && a.side) {
		while (final <= 5) {
			if (a.house[final] != 2 && a.house[final] != 3) break;
			captured += a.house[final]; final++;
		}
		if (captured == a.nSeeds) return true;
	}
	return false;
}

/** Proposes the user a draw.
Distributes seeds if he agrees.
@return user decision (TRUE if draw accepted, else FALSE).
*/
bool proposeDraw() {
	print("DRAW PROPOSAL", YELLOW); cout << "\n\n";
	int ans = askTwoOptions("The game seems to be going around in circles. What to do now?\n\n1. Draw - be happy\n2. Continue - risk it all\n\n");

	if (ans == 1) { //accept draw. distribute seeds
		clearScreen(); printBoard(false);
		a.score = 24; b.score = 24;
		for (int i = 0;i < 6;i++) {
			a.house[i] = 0; b.house[i] = 0;
		}
		print("DRAW PROPOSAL ACCEPTED!", YELLOW); cout << "\n";
		cout << "\nWe will distribute seeds so that each player scores 24.\n";
		waitEnter();
		return true;
	}
	else { //reject draw and continue
		clearScreen(); printBoard(false);
		b.wantsDraw = false;
		print("DRAW PROPOSAL REFUSED!", YELLOW); cout << "\n";
		cout << "\nYou have " << MOVE_LIMIT - b.nMoves << " moves left to avoid a forced draw.\n";
		waitEnter();
		clearScreen(); printBoard(); //bring messages back
		return false;
	}
}

/** Prints move limit message. Distribute seeds.
*/
void abortGame() {
	print("MOVE LIMIT REACHED!", YELLOW); cout << "\n";
	cout << "\n" << "More than " << MOVE_LIMIT << " moves have been played by each.\n";
	a.score = 24; b.score = 24;
	for (int i = 0;i < 6;i++) {
		a.house[i] = 0; b.house[i] = 0;
	}
	cout << "We will distribute seeds so that each player scores 24.\n";
	waitEnter();
}