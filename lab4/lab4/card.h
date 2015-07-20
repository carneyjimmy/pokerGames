//
//  card.h
//  Lab4
//
//  Created by Jared Eisenberg on 3/31/15.
//  Copyright (c) 2015 Jared Eisenberg. All rights reserved.
//

#ifndef __Lab4__card__
#define __Lab4__card__

#include <stdio.h>
#include <vector>

using namespace std;

struct Card {
	enum Suit { clubs, diamonds, hearts, spades };
	enum Rank { two = 2, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace };
	//Rank enumeration is manually set to start at 2 so that ranks 2-9 correspond to their enum values.
	Suit suit;
	Rank rank;
	bool operator<(const Card & card2) const;
};

//Functions for card parsing and printing
//int printCards(const vector<Card> & cardSet);

//Functions for card sorting and hand making
//int determineHands(const vector<Card> & deck);

#endif /* defined(__Lab4__card__) */
