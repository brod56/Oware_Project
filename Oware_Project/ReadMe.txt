Last updated: 31/03/2020

Submission 1:
- Initial version

Submission 2:
- Updated documentation

Submission 3:
- Fixed draw logic
- Interface changes

Submission 4:
- Fixed hard bot bug
- New hard bot behaviour: evaluates opponent's captures even if it has his own
- Special characters in players' names are now accepted

Submission 5:
- Bug fixes
- Prevented Grand Slams captures
---------------------------------------------------------------------
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