========================================================================
    CONSOLE APPLICATION : Lab4 Project Overview
========================================================================
Lab 4
Jimmy Carney, Jared Eisenberg, Brandon Meeks 



AppWizard has created this final4 application for you.

This file contains a summary of what you will find in each of the files that
make up your final4 application.


final4.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

final4.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

final4.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named final4.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Wrters:
Lab 4
Jimmy Carney, Jared Eisenberg, Brandon Meeks


/////////////////////////////////////////////////////////////////////////////
_________________________________________________________
Notes:	EXTRA CREDIT COMPLETED
		There is a max player limit at 7 players
		If all but one folds, we let the player remaining type-in any command, but nothing happens,
			 and then he wins automatically after all the fake-rounds are over

Trials:
A lot of it was running and playing the program again and again and making sure all the bets, chips, winners, and folding-logic was correct
Specifically, we tested:

all players (and all but one) leaving,
	which prompted the screen to make a new game

people joining after leaving,
	it added them normally

splitting the pot among winners
	which we made equally split the pot in a tie

everybody folding, 
	which keeps the game going for the remaining player, but no betting can take place, and then they win the bet

Having a player run out of chips, and rejoin with 20 chips
	player rejoins with 20 chips and continues playin

Having player run out of chips and quit
	player is removed from game
_______________________________________
Problems we ran into:

Five Card Draw:
We did Seven Card first with betting, so it was easy enough to implement betting into our already-made 5 card draw. 

Samll hitch with folding logic, but we were able to take care of it, it was just due to forgetting to reset the "isFolded" bool to false after each round

Seven Card Stud:

Our SevenCardStud class has the 6 pure virtual methods from the Game class, as does five card draw and Hold 'em.
before_turn handles dealing cards to players (only if they haven't folded).
turn handles giving the player a status update as well as handling their bet.
after_turn handles collecting money for the pot, and resetting players' betting attributes
before_round handles resetting the pot, getting antes and shuffling the deck
round utilizes an integer "phase" that tracks which turn in the round it is.
    For 5 phases, players' before_turn, turn and after_turn methods will be called
    The turn method is inside a loop that continues to execute until betsCleared evaluates to true.
after_round handles determining a winner, resets the deck, handles players leaving and joining,
    resets the pot, increments dealer position, and prints round results

We were forced to rework how we dealt with determining the rank for a player at the end of a round.
Due to the possibility of having seven cards, we created a generateBestHand function.
This function in the Hand class checks every permutation of a player's hand evaluating the first 5 cards in the hand as a separate hand to find the best 5 card combination of cards in a player's hand.  The resulting hand is returned.
We ended up rewriting several methods in our Hand class including the poker_rank method which now uses several helper methods to break ties and the determineRank method which now uses several helper methods to determine a Hand's rank.

Texas Hold Em:
We were using the "resetDeck" method from 7 card stud for this class, but soon realized that didnt work, so we made a resetTexas method

We had a lot of trouble figuring out how to discard all of the cards (because of the shared hand),
We solved this by making a new hand that takes all the cards that are dealt either to a player at first (even if a player folds they get two cards) as well as the shared cards,
and just clearing the pocket-hands and shared cards.

It took a while to get the folding logic down, if one player leaves. we solved this by letting the player remaining type in any command, but nothing happens, and then he wins automatically

Betting:
Our betting functionality is based around 3 functions: takeAction, performAction, and betsCleared.
takeAction prompts the user for an action, listing the possible choices the player has.  It then filters user input
    until receiving a valid action request.  It then returns an int indicating which action has been selected.
This integer will be used as the input parameter for the performAction method, which alters players' isCalled and isFolded values
    depending on the action parameter.
After a user bets using the above two methods, betsCleared checks to see if the betting round is over and returns a corresponding boolean.
The logic for betsCleread is largely based on the isFolded and isCalled values for each player.


Other:
While testing for correctness, we figured out that when someone got a pair, his best cards weren't shown (for the gaes with 7 cards)
We went back and realized that during our restructure of poker-rank, we switched some '<' signs on the pair tie break method.

____________________________
Warning:
Nothing besides plenty of unsigned int / int conversion warnings, which just takes some tedious fixing

Errors:
On the five-card-draw class We got a very weird "binary operator '==' " error (C2678) that unfortuantely didnt give us a line number
We solved this by downloading outr lab 3 code and re-implementing the betting, and fortunately this worked.

While testing teas hold em and sevenCardStud, we got a abort- invalid operator '<' error, and using the debugger figured out it was in poker rank
We saw that one of the poker rank if statements had, basically (if i < j) return true, else if (i < j) return false.
Obviosuly, we had to flip the incorrect operator