#include "oware.h"

extern Player a; extern Player b;

/** Updates both player's win attribute if they won/drawn.
If both have 24 seeds, both will "win".
@return TRUE if a win/draw has occured, else FALSE.
*/
bool hasWinner() {
	if (a.score > 24) {
		a.win = true; return true;
	}
	else if (b.score > 24) {
		b.win = true; return true;
	}
	else if (a.score == 24 && b.score == 24) { //both 24 is a draw. check main
		a.win = true; b.win = true; return true;
	}
	else return false;
}

/** Checks both players' attribute mayRemoveAllPieces.
@param remove - if TRUE, remove the pieces of the player in the situation to do so, else don't.
@return TRUE if one true attribute found.
*/
bool cannibalize(bool remove) {
	if (a.mayRemoveAllPieces) {
		if (remove) {
			a.score += a.nSeeds;
			for (int i = 0; i < 6; i++) {
				a.house[i] = 0;
				updatenSeeds(); hasWinner();
			}
		}
		return true;
	}

	else if (b.mayRemoveAllPieces) {
		if (remove) {
			b.score += b.nSeeds;
			for (int i = 0; i < 6; i++) {
				b.house[i] = 0;
				updatenSeeds(); hasWinner();
			}
		}
		return true;
	}

	else return false;
}

/** Checks if a draw offer or a forced draw should happen.
@return 0 when nothing, 1 when draw offer, 2 when forced draw
*/
int eligibleDraw() {
	if (b.nMoves >= MOVE_LIMIT && !hasCaptures(b) && !hasCaptures(a) && a.nSeeds <= M_DRAW_SEEDS * 2 && b.nSeeds <= M_DRAW_SEEDS * 2)
		return 2; //forced draw
	if (b.nMoves >= DRAW_PROPOSAL && !hasCaptures(b) && !hasCaptures(a) && a.nSeeds <= M_DRAW_SEEDS && b.nSeeds <= M_DRAW_SEEDS && b.wantsDraw)
		return 1; //draw proposal
	return 0; //game must continue
}

/** Checks if a move is possible:
- must be inside the board
- must not be a empty house
- must give the opponent seeds if they hasn't got any
@param p - player to move.
@param intitial - the house to play from, to be evaluated.
@param output - if TRUE, will display warnings, else won't.
@return TRUE if the move is legal, else FALSE.
*/
bool possibleMove(Player p, int initial, bool output) {
	if (!p.turn) output = false;

	if (!inBounds(initial)) {
		if (output) cout << "\n" << "The house " << initial << " does not exist.\n";
		return false;
	}

	if (p.nSeeds == 0) return true; //allow bestMove() to randomize when opponent has no seeds

	//check if no seeds in house
	if (p.house[initial] == 0) {
		if (output) cout << "\n" << "The house " << initial << " has no seeds. \n";
		return false;
	}

	//"let the opponent play" rule
	if (p.charName == 'A' && b.nSeeds == 0) {
		if (initial - a.house[initial] >= 0) { //doesnt give opponent any seeds
			//check if any possible moves left
			for (int i = 5; i >= 0; i--) {
				if (i - a.house[i] < 0) {
					if (output)
						cout << "\n" << "As of now, " + b.name + " has no seeds. Make sure you sow in his camp.\n";
					return false; //there are possible moves left; force other move
				}
			}
			if (a.turn) a.mayRemoveAllPieces = true; // let him play, then abort game
			return true;
		}
		else return true;
	}

	else if (p.charName == 'B' && a.nSeeds == 0) {
		if (initial + b.house[initial] <= 5) {
			for (int i = 0; i <= 5; i++) {
				if (i + b.house[i] > 5) {
					if (output)
						cout << "\n" << "As of now, " + a.name + " has no seeds. Make sure you sow in his camp.\n";
					return false;
				}
			}
			if (b.turn) b.mayRemoveAllPieces = true;
			return true;
		}
		else return true;
	}

	//if all is ok
	return true;
}

/** Checks if the house exists (is inside the board).
@param house - house to be evaluated.
*/
bool inBounds(int house) {
	if (house < 0 || house > 5) return false;
	return true;
}

/** Negates both players' turn attribute.
*/
void switchTurns() {
	if (a.turn && !b.turn) {
		a.turn = false; b.turn = true;
	}
	else if (b.turn && !a.turn) {
		b.turn = false; a.turn = true;
	}
}

/** Negates both player's side attribute.
*/
void switchSides() {
	if (a.side && !b.side) {
		a.side = false; b.side = true;
	}
	else if (b.side && !a.side) {
		b.side = false; a.side = true;
	}
}

/** Makes both player's nSeeds attribute the updated sum of the seeds in their houses.
*/
void updatenSeeds() {
	a.nSeeds = 0; b.nSeeds = 0;
	for (int i = 0;i < 6;i++) {
		a.nSeeds += a.house[i]; b.nSeeds += b.house[i];
	}
}
