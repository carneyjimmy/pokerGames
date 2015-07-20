//
//  main.cpp
//  Lab4
//
//

#include "stdafx.h"
#include "game.h"
#include <iostream>
#include <string>
#include <time.h>

using namespace std;

int usage(string message, int value){
	cerr << message << " (Error Code " << value << ")" << endl;
	return value;
}

int main(int argc, const char * argv[]) {
	if (argc < 4){ //too small
		return usage("Too few arguments entered. Please run program with the name of a game, and two or more players", 6);
	}
	else{
		try {
			Game::start_game(argv[1]);
		}
		catch (int errorCode){
			return usage("Exception thrown: ", errorCode);
		}
		shared_ptr<Game> gamePtr = Game::instance(); //pointer of game
		for (int arg = 2; arg < argc; ++arg){
			(*gamePtr).add_player(argv[arg]);
		}
		bool multPlayers = true; //are there at least two players? if so run program until their arent
		srand(time(NULL));
		while (multPlayers){

			try{

				(*(gamePtr)).before_round();

				int j = (*(gamePtr)).round();
				if (j == 15){
					return usage("There are not enough cards to support this game", j);
				}

				(*(gamePtr)).after_round();


			}
			catch (int errorCode){
				return usage("Exception thrown: ", errorCode);
			}

			if ((*gamePtr).crowdSize() < 2){
				multPlayers = false;
			}
		}
		try {
			(*gamePtr).stop_game();
		}
		catch (int errorCode){
			return usage("Exception thrown: ", errorCode);
		}
		cout << "GAME ENDING" << endl;
	}
	return COMPLETION_SUCCESS;
}