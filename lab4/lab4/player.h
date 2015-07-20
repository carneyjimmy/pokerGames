//
//  player.h
//  Lab4
//

#ifndef __Lab4__player__
#define __Lab4__player__

#include <stdio.h>
#include "hand.h"
#include <string>

struct Player{

	std::string name; //the name of the player
	Hand hand; //the player's hand object
	unsigned int wins; //number of wins
	unsigned int losses; //number of losses
	unsigned int chips; //number of chips a player possesess
	unsigned int cumulativeBet;
	bool isFolded;
	bool onAuto; //a boolean indicating whether or not the player is automated

	Player(const char * inputName); //player constructor

};

ostream & operator<<(ostream & out, Player p); //operator to print a player's info to an ostream

#endif /* defined(__Lab4__player__) */
