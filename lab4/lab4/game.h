//
//  game.h
//  Lab4
//

#ifndef __Lab4__game__
#define __Lab4__game__

#include <stdio.h>
#include "Lab4.h"
#include "deck.h"
#include "player.h"
#include <memory>
#include <vector>


class Game{


protected:
	static shared_ptr<Game> ptr; //shared_ptr to point at a Game object
	Deck mainDeck; //a deck object to serve as the deck of cards for the game
	vector<shared_ptr<Player>> playerVector; //a vector of players who are currently playing the game
public:
	Game(); //default constructor
	virtual ~Game(); //destructor
	static shared_ptr < Game >  instance(); //used to obtain the protected ptr variable
	static void start_game(string s); //the function called to begin a game
	static void stop_game(); //the function called to end a game
	size_t crowdSize(); //a function to obtain the current number of active players

	int add_player(const string & newPlayer); //a function to add a player to the current game
	shared_ptr<Player> find_player(const string & playerToFind); //a function used to find a player in the game

	//virtual function declarations to be defined in derived classes
	virtual int before_turn(Player & player) = 0;
	virtual int turn(Player & player) = 0;
	virtual int after_turn(Player & player) = 0;
	virtual int before_round() = 0;
	virtual int round() = 0;
	virtual int after_round() = 0;
	virtual void ante() = 0;




};

#endif /* defined(__Lab4__game__) */
