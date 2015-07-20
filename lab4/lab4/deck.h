//
//  deck.h
//  Lab4
//
//  Created by Jared Eisenberg on 3/31/15.
//  Copyright (c) 2015 Jared Eisenberg. All rights reserved.
//

#ifndef __Lab4__deck__
#define __Lab4__deck__

#include <stdio.h>
#include "card.h"
#include "hand.h"
#include <vector>
#include <string>

class Hand;

class Deck{

	friend int operator<<(ostream & o, const Deck & d);
	friend int operator<<(Hand & h, Deck & d);
	friend int operator<<(Deck & destination, Deck & start);

public:
	Deck(const char * cardFile); //default constructor
	Deck(); //default constructor to make empty deck
	int load(const char * cardFile); //load method for reading in card definition strings and inserting into Deck
	int shuffle(); //shuffle method for shuffling the deck using iterators and shuffle algorithm
	int size() const; //size method returns the size of the deck (# of cards in it)
	int addCard(Card & c); //add card method adds a card to the deck container
	void checkSuit(string & word, Card & nextCard);	//check is a given string is a valid card definition
	Card::Rank Deck::assignRank(string & word); //returns a rank given a valid card string
	Card::Suit Deck::assignSuit(string & word); //returns a suit given a valid card string

private:
	std::vector<Card> stack;


};

int operator<< (ostream & o, const Deck & d);


#endif /* defined(__Lab4__deck__) */
