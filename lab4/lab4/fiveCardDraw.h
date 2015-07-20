//
//  fiveCardDraw.h
//

#ifndef __Lab4__fiveCardDraw__
#define __Lab4__fiveCardDraw__

#include <stdio.h>
#include "game.h"

//a derived class from the Game base class
class FiveCardDraw : public Game{

protected:
	size_t dealer; //an integer to keep track of which player is the current dealer
	Deck discardPile = Deck(); //a deck object to store discarded cards
	unsigned int pot;
public:
	FiveCardDraw(); //default constructor
	unsigned int currentBetTotal;
	int FiveCardDraw::getHighestCumulative();
	virtual void ante();//ante up
	virtual int before_turn(Player & p); //a function to get the desired discards from player p
	virtual int turn(Player & player); //a function to replace discarded cards in player's hand
	virtual int after_turn(Player & player); //a function to print out the resulting player's hand
	virtual int before_round(); //function to control each player's before_turn function and deal cards out to each player
	virtual int round(); //function to call each player's turn and after_turn sequence
	virtual int after_round(); //function to determine the winner of each hand and deal with leaving and joining players
	void processBet(Player & p, string & bet);	//function which takes in a bet, validates, and adds to the port
	string takeBet(Player & p);
	void FiveCardDraw::processInput(vector<size_t> & discards);	//function which processes user input and dictates appropriate logic
	int removePlayer(string playerName);
	bool is_number(const std::string& s);
};

bool pokerRank(shared_ptr<Player> p1, shared_ptr<Player> p2); //a function to determine which of two players has a better hand
int operator<<(Deck & destination, Deck & start); //an operator to shift all cards from one deck to another

#endif /* defined(__Lab4__fiveCardDraw__) */
