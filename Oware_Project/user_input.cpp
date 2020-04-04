#include "oware.h"
#include <cstddef>
#include <cctype>

/** Prompts user to press enter and waits for it.
*/
void waitEnter() {
	cout << "\n" << "Press enter to continue...\n";
	cin.ignore(10000, '\n');
}

/** Present the user two choices.
@param message - message to be displayed if no correct input
@return user option (1 or 2)
*/
int askTwoOptions(string message) {
	int ans;
	cout << message;

	while (!(cin >> ans) || (ans != 1 && ans != 2)) {
		cout << "\n" << "Please type 1 or 2 according to your preference. ";
		if (cin.fail()) cin.clear();
		cin.ignore(10000, '\n');
	}
	cin.ignore(10000, '\n');

	cout << "\n";
	return ans;
}

/** Prints message and ask for name input.
@param message - message to be displayed.
@return formatted name (no spaces, cappital initials)
*/
string askName(string message) {
	string name;
	for (;;) { //breaks when name length is short enough and not empty
		cout << message; getline(cin, name);

		//prevent empty names
		if (name == "")
			cout << "\n" << "Player names cannot be empty. Please try again.\n";
		else {
			name = formatName(name);
			//ensure no large names or unrecognized chars
			if (name.length() > NAME_LENGTH_LIMIT)
				cout << "\n" << "Please do not input large names. Try again.\n";
			else break;
		}
	}
	return name;
}

/** Strip unnecessary spaces and uppercase all initials.
@param name - string to be treated.
@return formatted string.
*/
string formatName(string name) {
	//strip spaces
	for (size_t i = 0; i < name.length(); i++) {
		if (name.at(i) == ' ') {
			if (i == name.length() - 1) {
				name.erase(i, 1);
				break;
			}
			else if (i == 0 || name.at(i + 1) == ' ') {
				name.erase(i, 1);
				i--;
			}
		}
	}

	//upper initials
	bool doUpper = true;
	for (size_t i = 0; i < name.length(); i++) {
		if (doUpper && name.at(i) >= 'A' && name.at(i) <= 'Z') doUpper = false; //already uppercase
		else if (doUpper && !(name.at(i) >= 'A' && name.at(i) <= 'Z')) {
			name.at(i) = toupper(name.at(i));
			doUpper = false;
		}
		else if (name.at(i) == ' ') doUpper = true;
	}

	return name;
}