//
//  hand.h
//  Lab4
//

#ifndef __Lab4__hand__
#define __Lab4__hand__

#include <stdio.h>
#include "card.h"
#include "deck.h"
#include <vector>
#include <string>

class Deck;

class Hand{

	friend int operator<<(ostream & o, const Hand & h);
	friend int operator<<(Hand & h, Deck & d);
	friend bool poker_rank(const Hand & h1, const Hand & h2);
	friend int determineRank(const vector<Card> & hand);



public:
	Hand(); // default constructor

	Hand(const Hand & h); // copy constructor

	int operator=(const Hand & h); // assignment operator

	int size() const; // size method returns number of cards in member container

	bool operator==(const Hand & h) const; // equivalence operator

	bool operator<(const Hand & h) const; // less than operator

	string stringify() const; // "as string" method that returns c++ style string representing the hand (e.g. "6H 7C 8D 9H JS")

	Card & operator[](size_t index); // indexing operator returns card object at specific index in hand

	int removeCard(size_t index); // remove card method removes card object from hand container

	vector<Card> & getHand();

	enum rank{ no_rank, pair, two_pair, three_kind, straight, flush, full_house, four_kind, straight_flush };

private:
	vector<Card> playerHand;

};



int operator<<(ostream & o, const Hand & h);
int operator<<(Hand & h, Deck & d);
bool poker_rank(const Hand & h1, const Hand & h2);
int determineRank(const vector<Card> & hand);


#endif /* defined(__Lab4__hand__) */
