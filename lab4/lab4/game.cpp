//
//  game.cpp
//

#include "stdafx.h"
#include "game.h"
#include "fiveCardDraw.h"
#include "player.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

//initialize ptr to be singular
shared_ptr<Game> Game::ptr = nullptr;

//default constructor
Game::Game(){
	mainDeck = Deck();
};

//default destructor
Game::~Game(){

}

//instance method serves as accessor for ptr member variable
shared_ptr<Game> Game::instance(){
	return ptr;
}

//start game function deals with starting correct game
void Game::start_game(string s){

	//if ptr is not singular, a game is already active so another cannot be started
	if (ptr != nullptr){
		cerr << "There is already a game in progress." << endl;
		throw GAME_ALREADY_STARTED;
	}

	else{
		//if the given game is not Five Card Draw, throw an error, since this program only supports that game
		if (s == "FiveCardDraw"){
			ptr = make_shared<FiveCardDraw>();

		}
		else{
			cerr << "Invalid game type given." << endl;
			throw UNKNOWN_GAME;
		}
	}
}

//function used to end running games
void Game::stop_game(){
	if (ptr == nullptr){
		cerr << "Tried to stop a game, but no games currently running." << endl;
		throw NO_GAME_IN_PROGRESS;
	}
	else{
		ptr = nullptr;
	}

}

//function used to add a player to the current game
int Game::add_player(const string & newPlayer){

	//check to see if the name is already playing
	for (unsigned int i = 0; i < playerVector.size(); ++i){
		if (newPlayer == (*(playerVector[i])).name) {
			cerr << "That player is already in the game." << endl;
			throw ALREADY_PLAYING;
		}
	}

	//add a new shared_ptr to player to the player vector
	const char * inputName = newPlayer.c_str();
	shared_ptr<Player> newPtr = make_shared<Player>(inputName);
	playerVector.push_back(newPtr);
	return COMPLETION_SUCCESS;

}

//returns the number of players currently playing the game
size_t Game::crowdSize(){
	return playerVector.size();
}

//returns a player identified by the name given as input
shared_ptr<Player> Game::find_player(const string & playerToFind){
	for (unsigned int i = 0; i < playerVector.size(); ++i){
		if (playerToFind == (*(playerVector[i])).name){
			return playerVector[i];

		}
	}
	return nullptr;
}

