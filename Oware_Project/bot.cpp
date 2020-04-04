#include "oware.h"

extern Player a; extern Player b;

/** Calculates the best move a player has in the moment.
Evaluates own and opponent's captures.
@param operation - player to move.
@param preventCaptures - if FALSE, will randomize if no offensive captures. should be false in recursive calls
@return best when captures/runs found, otherwise a possible random move
*/
int bestMove(Player p, bool preventCaptures) {
	vector<int> scores; int move = - 1; bool randomize = false;

	//check for offensive captures
	for (int i = 0; i < 6; i++) {
		if (!possibleMove(p, i, false)) {
			scores.push_back(-1);
		}
		else scores.push_back(calculateScore(p, i));
	}
	move = maxElementIndex(scores);

	if (preventCaptures) { // do not enter this in recursive calls. infinite loop
		int candidate = 0; int final = -1; int off_score = 0; int deff_score = 0;

		if (p.charName == 'A') {
			candidate = bestMove(b, false);
			final = calculateFinal(b, candidate);
			off_score = calculateScore(a, move);
			deff_score = calculateScore(b, candidate);

			if (a.score + off_score <= 24) {
				if (deff_score > off_score || b.score + deff_score >= 24) move = final;
				else if (off_score <= 0) randomize = true;
			}
		}

		else if (p.charName == 'B') {
			candidate = bestMove(a, false);
			final = calculateFinal(a, candidate);
			off_score = calculateScore(b, move);
			deff_score = calculateScore(a, candidate);

			if (b.score + off_score <= 24) {
				if (deff_score > off_score || a.score + deff_score >= 24) move = final;
				else if (off_score <= 0) randomize = true;
			}
		}
	}

	while (!possibleMove(p, move, false) || randomize) { // do not attempt to move from empty house
		move = rand() % 6; randomize = false;
	}

	return move;
}

/** Calculate captured seeds.
@param player - player to move.
@param initial - house the player chooses to play from.
@return -1 if move doesn't reach opponent side, else captured
*/
int calculateScore(Player p, int initial) {
	int captured = 0;
	int final = calculateFinal(p, initial);
	if (!inBounds(initial) || final < 0) return -1; //won't reach opponent's side

	if (p.charName == 'A') {
		while ((b.house[final] + 1 == 3 || b.house[final] + 1 == 2) && final >= 0) {
			captured += b.house[final] + 1;
			final--;
		}
	}

	else if (p.charName == 'B') {
		while ((a.house[final] + 1 == 3 || a.house[final] + 1 == 2) && final <= 5) {
			captured += a.house[final] + 1;
			final++;
		}
	}
	return captured;
}

/** Calculates final sowed house on the opponent's camp.
@param player - player to move.
@param intitial - house he plays from.
@return final house if opponent's camp, else -1
*/
int calculateFinal(Player p, int initial) {
	if (!inBounds(initial)) return -1;

	if (p.charName == 'A') {
		if (initial - a.house[initial] >= 0) return -1;
		if (initial - a.house[initial] < -6) return -1;
		return abs(initial - a.house[initial] + 1);
	}

	else {
		if (initial + b.house[initial] > 11) return -1;
		if (initial + b.house[initial] <= 5) return -1;
		return 5 - ((initial + b.house[initial] - 1) % 5);
	}
}

/** Finds the index of the max positive value in a vector.
@param v - vector to be iterated.
@return if two+ maxes, highest index; if one max, its index; else -1
*/
int maxElementIndex(vector<int> v) {
	int max_n = -1; int max_i = 0;

	for (size_t i = 0;i < v.size(); i++) {
		if (v.at(i) >= max_n) {
			max_n = v.at(i);
			max_i = i;
		}
	}
	if (max_n <= 0) return -1;
	return max_i;
}

/** Check if a player can capture any seeds assuming he's to move.
@param p - player to move.
@return TRUE when has captures, else FALSE.
*/
bool hasCaptures(Player p) {
	for (int i = 0;i < 6;i++) {
		if (possibleMove(p, i, false)) {
			if (calculateScore(p, i) > 0) {
				return true;
			}
		}
	}
	return false;
}