//
//  fiveCardDraw.cpp
//

#include "stdafx.h"
#include "fiveCardDraw.h"
#include "card.h"
#include "hand.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>


//default constructor
FiveCardDraw::FiveCardDraw(){

	//set the dealer to start at the first player
	(*this).dealer = 0;
	pot = 0;
	currentBetTotal = 0;

	//iterate through all possible card rank/suit combinations adding one of each to the main deck
	for (int s = Card::clubs; s <= Card::spades; ++s){
		for (int r = Card::two; r <= Card::ace; ++r){
			Card c;
			c.suit = static_cast<Card::Suit>(s);
			c.rank = static_cast<Card::Rank>(r);
			(*this).mainDeck.addCard(c);
		}
	}
}
//ante up, each player gives on chip to the pot
void FiveCardDraw::ante(){
	for (unsigned int i = 0; i < playerVector.size(); ++i){
		(*(playerVector[i])).chips--;
		pot++;
	}
}

int FiveCardDraw::before_turn(Player & p){
	std::cout << p.name << ": " << p.hand.stringify() << " " << endl;

	//declare a vector that will hold the indexes of the cards to be discarded
	vector<size_t> discards;


	//first round of betting
	cout << "FIRST ROUND: " << endl;
	std::cout << p.name << ", how much do you want to bet?" << endl;
	std::cout << "the current bet is " << currentBetTotal << "." << endl;
	std::cout << "if there is no current bet, you can either check or type one or two to bet the coresonding chips" << endl;
	std::cout << "if there is a current bet, you can either call or type 'raise 1' or 'raise 2' to bet the corresponding amount" << endl;



	//check and process bet
	string bet = takeBet(p);
	processBet(p, bet);

	processInput(discards);	//process any discard commands

	//sort the discard entries
	std::sort(discards.begin(), discards.end());

	//discard cards from the players hand in descending order (to prevent abnormal behavior due to cards shifting)
	for (unsigned int index = 0; index < discards.size(); ++index){
		discardPile.addCard(p.hand[discards[discards.size() - index - 1]]);
		p.hand.removeCard(discards[discards.size() - index - 1]);
	}
	return COMPLETION_SUCCESS;
}



string FiveCardDraw::takeBet(Player & p){
	string input;

	if (p.chips > 0){

		bool validInput = false;

		while (!validInput){
			getline(cin, input);

			if (input != "1" && input != "2" && input != "check" && input != "call" && input != "fold" && input != "raise 1" && input != "raise 2"){
				cout << "Please make a input: check, call, raise 1, raise 2, or fold" << endl;
			}
			else if (input == "check" && pot > 0){
				std::cout << "Cannot check when there is a pot" << endl;
			}
			else if (input == "call" && currentBetTotal == 0){
				cout << "There is no bet, so you cannot call" << endl;
			}
			else if (input == "call" && (getHighestCumulative() - p.cumulativeBet) > p.chips){ //if not enough chips to call
				cout << "You don't have enough chips to call" << endl;
			}
			else if (input == "1" || input == "2"){	//then must be a bet input
				unsigned int betInt = stoi(input);
				if (currentBetTotal > 0){
					cout << "You must raise, call, or fold if there is a current bet - did you mean to type 'raise 1' or 'raise 2?" << endl;
				}
				else if (betInt > p.chips){
					cout << "You don't have enough chips for that bet!" << endl;
				}
				else{
					validInput = true;
				}
			}
			else if (input == "raise 1"){	//raise 1
				if (currentBetTotal == 0){
					cout << "There are no bets to raise!" << endl;
				}
				else if (p.chips < (getHighestCumulative() - p.cumulativeBet) + 1){
					cout << "You don't have enough chips for that raise!" << endl;
				}
				validInput = true;
			}
			else if (input == "raise 2"){	//raise 2
				if (currentBetTotal == 0){
					cout << "There are no bets to raise!" << endl;
				}
				else if (p.chips < (getHighestCumulative() - p.cumulativeBet) + 2){
					cout << "You don't have enough chips for that raise!" << endl;
				}
				validInput = true;
			}
			else if (input == "fold" && pot == 0){
				cout << "Nobody has bet, you can not fold" << endl;

			}
			else{
				validInput = true;
			}
		}
	}
	else{	//player has no chips
		cout << "Player has no chips, automatically skipping..." << endl;
	}

	return input;
}

int FiveCardDraw::getHighestCumulative(){
	vector<int> bets;
	for (unsigned int increment = 1; increment <= playerVector.size(); ++increment){
		Player & nextPlayer = *playerVector[(dealer + increment) % playerVector.size()];
		bets.push_back(nextPlayer.cumulativeBet);
	}
	return *max_element(bets.begin(), bets.end());
}

void FiveCardDraw::processBet(Player & p, string & input){
	if (input == "check"){
		cout << "Player checked..." << endl;
	}
	else if (input == "call"){
		unsigned int amountCall = getHighestCumulative() - p.cumulativeBet;
		p.chips -= amountCall;
		p.cumulativeBet += amountCall;
		currentBetTotal += amountCall;
		pot += amountCall;
	}
	else if (input == "1" || input == "2"){	//must be a bet
		int betInt = stoi(input);
		p.chips -= betInt;
		p.cumulativeBet += betInt;
		pot += betInt;
		currentBetTotal += betInt;
	}
	else if (input == "raise 1"){
		unsigned int amountRaise = getHighestCumulative() - p.cumulativeBet + 1;
		p.chips -= amountRaise;
		p.cumulativeBet += amountRaise;
		pot += amountRaise;
		currentBetTotal += amountRaise;

	}
	else if (input == "raise 2"){
		unsigned int amountRaise = getHighestCumulative() - p.cumulativeBet + 2;
		p.chips -= amountRaise;
		p.cumulativeBet += amountRaise;
		pot += amountRaise;
		currentBetTotal += amountRaise;
	}
	else{	//player must have folded
		p.isFolded = true;
	}
}



void FiveCardDraw::processInput(vector<size_t> & discards){

	bool validInput = false;	//declare a bool to track whether a valid input has been received
	//continue to iterate until the user provides a valid input
	while (validInput == false){

		//clears the discards vector in case there is data from a previous invalid input
		discards.clear();

		//set the bool to true until proven otherwise (below)
		validInput = true;

		std::cout << "What cards would you like to discard? (Leave blank if none)" << endl;
		string userInput;

		//get the user's input
		getline(std::cin, userInput);
		istringstream iss(userInput);
		string nextWord;
		size_t nextNum;

		//while there are more digits to read from the user input...
		while (iss >> nextWord){

			//if the next word is a valid digit
			if (nextWord.size() == 1 && isdigit(nextWord[0])){
				nextNum = stoi(nextWord);

				//check to make sure the digit is in range
				if (nextNum > 4){
					std::cout << "Out of Range Input found" << endl;
					validInput = false;
				}

				//check to make sure no duplicats are found
				else if (find(discards.begin(), discards.end(), nextNum) == discards.end()){
					discards.push_back(nextNum);
				}

				//if duplicate values are found...
				else {
					validInput = false;
					std::cout << "Duplicate values found" << endl;
				}
			}

			//if a non-digit or number with more than one digit is found
			else {
				std::cout << "Invlid input found" << endl;
				validInput = false;
			}
		}

		//flush the stringstream
		iss.clear();

		//if there are too many input values (should not be possible given the checks above)
		if (discards.size() > 5){
			std::cout << "Too many values given" << endl;
			validInput = false;
		}
	}
}

//a function that takes care of replaing discarded cards in player's hands
int FiveCardDraw::turn(Player & player){

	if (player.isFolded){
		cout << player.name << " has folded" << endl;
	}
	else{
		//determine the number of cards the player needs
		size_t cardsNeeded = 5 - player.hand.size();

		//deal the player that many cards from the main deck
		for (unsigned int i = 0; i < cardsNeeded; ++i){
			if (mainDeck.size() > 0){
				player.hand << mainDeck;
			}
		}

		//if the main deck was unable to deal the right amount of cards, use the discard pile
		if (player.hand.size() != 5){
			discardPile.shuffle();
			for (int i = player.hand.size(); i < 5; ++i){
				if (discardPile.size() == 0){
					return NO_CARDS_LEFT;
				}
				player.hand << discardPile;
			}
		}

		//second round of betting
		cout << "SECOND ROUND:" << endl;
		std::cout << player.name << ", how much do you want to bet?" << endl;
		std::cout << "the current bet is " << currentBetTotal << "." << endl;
		std::cout << "if there is no current bet, you can either check or type one or two to bet the coresonding chips" << endl;
		std::cout << "if there is a current bet, you can either call or type 'raise 1' or 'raise 2' to bet the corresponding amount" << endl;

		//check and process bet
		string bet = takeBet(player);
		processBet(player, bet);
	}


	return COMPLETION_SUCCESS;
}

//function for displaying results after a player's turn
int FiveCardDraw::after_turn(Player & p){
	std::cout << p.name << ": " << p.hand.stringify() << " " << endl;
	return COMPLETION_SUCCESS;
}

//function for organizing the beginning and discarding portion of a round
int FiveCardDraw::before_round(){
	pot = 0;
	currentBetTotal = 0;
	//shuffle the deck
	mainDeck.shuffle();

	//deal cards to every player until they all have 5 cards starting with the player after the dealer
	for (int cardNum = 0; cardNum < 5; ++cardNum){
		for (unsigned int increment = 1; increment <= playerVector.size(); ++increment){
			playerVector[(dealer + increment) % playerVector.size()]->hand << mainDeck;
		}
	}

	//call before_turn on each player starting with the player after the dealer
	for (unsigned int increment = 1; increment <= playerVector.size(); ++increment){
		Player & nextPlayer = *playerVector[(dealer + increment) % playerVector.size()];
		nextPlayer.isFolded = false;
		nextPlayer.cumulativeBet = 0;
		before_turn(nextPlayer);
	}
	return COMPLETION_SUCCESS;

}

//function designed to manage the turn and after_turn calls for each player
int FiveCardDraw::round(){

	//first check if all but one folded
	//call before_turn on each player starting with the player after the dealer
	int foldCount = 0;
	for (unsigned int increment = 1; increment <= playerVector.size(); ++increment){
		Player & nextPlayer = *playerVector[(dealer + increment) % playerVector.size()];
		if (nextPlayer.isFolded){
			foldCount++;
		}
	}

	if (foldCount == playerVector.size() - 1){	//all but one has folded
		//now find the player who didn't
		for (unsigned int increment = 1; increment <= playerVector.size(); ++increment){
			Player & nextPlayer = *playerVector[(dealer + increment) % playerVector.size()];
			if (!nextPlayer.isFolded){
				//nextPlayer is the winner...
				cout << "Everybody but " << nextPlayer.name << " has folded..." << endl;
			}
		}
	}
	else{	//if not commence final round of betting

		std::cout << "FINAL HANDS" << endl;

		//call turn on each player followed by after_turn
		for (unsigned int increment = 1; increment <= playerVector.size(); ++increment){

			Player & nextPlayer = *playerVector[(dealer + increment) % playerVector.size()];

			int success = turn(nextPlayer);

			if (success != 0){
				return NO_CARDS_LEFT;
			}

			after_turn(nextPlayer);
		}
		std::cout << endl;

	}
	return COMPLETION_SUCCESS;
}

//function designed to handle the end of a round, determining results, quitters, and new players
int FiveCardDraw::after_round(){
	currentBetTotal = 0;
	//create a temporary vector for sorting (so as not to mix up the order of players at the table)
	vector<shared_ptr<Player>> roundResult = playerVector;

	//sort the players by hand rank
	sort(roundResult.begin(), roundResult.end(), pokerRank);

	//increment the best player's number of wins
	(*roundResult[playerVector.size() - 1]).wins++;
	(*roundResult[playerVector.size() - 1]).chips += pot;
	pot = 0;

	//increment everybody else's number of losses
	for (unsigned int player = 0; player < roundResult.size() - 1; ++player){
		(*roundResult[player]).losses++;
	}

	//display the results for the round, starting with the player with the best hand
	std::cout << "ROUND RESULTS" << endl;
	for (unsigned int player = 0; player < roundResult.size(); ++player){
		Player nextPlayer = *roundResult[roundResult.size() - player - 1];
		std::cout << nextPlayer << " " << nextPlayer.hand;
	}
	std::cout << endl;

	//move all cards back to the main deck
	for (unsigned int player = 0; player < roundResult.size(); ++player){
		for (int card = 1; card <= 5; ++card){
			mainDeck.addCard((*roundResult[player]).hand[5 - card]);
			(*roundResult[player]).hand.removeCard(5 - card);
		}

	}
	mainDeck << discardPile;

	//this section of the method handles players leaving and joining the game
	bool playersLeft = false;

	//continue to run until notified that all players leaving have successfully left
	while (!playersLeft){
		vector<string> quitters;
		for (unsigned int player = 0; player < roundResult.size(); ++player){
			if ((*roundResult[player]).chips == 0){ // is any player out of chips?
				std::cout << (*roundResult[player]).name << " has no more chips. Please type Reset to reset your hip count to 20, or type quit to exit the game" << endl;
				string brokePlayer;
				std::cin >> brokePlayer;

				if (brokePlayer == "quit"){
					quitters.push_back(brokePlayer);
				}
				else{
					(*roundResult[player]).chips = 20;
				}
			}
		}
		for (unsigned int i = 0; i < quitters.size(); ++i){
			for (unsigned int player = 0; player < playerVector.size(); ++player){
				if ((*playerVector[player]).name == quitters[i]){


					//if a non-automated player is successfully identified, log their results in their info file
					Player & quitter = *playerVector[player];
					int quitterIndex = player;
					string outFile = quitters[i] + ".txt";
					ofstream out(outFile);
					out << quitter << endl;
					out.close();
					playerVector.erase(playerVector.begin() + quitterIndex);
				}
			}
		}


		//Prompt the user for the next quitter, and store their name in a string
		std::cout << "Is there a player that would like to leave the game? (Enter their name or 'no')" << endl;
		string nextName;
		std::cin >> nextName;

		//if no more players are leaving
		if (nextName == "no"){
			playersLeft = true;
		}

		//if more players are leaving
		else {
			bool playerExists = false;
			int quitterIndex = -1;

			//search for the inputted name, and if found, check to make sure the player is not automated
			for (unsigned int player = 0; player < playerVector.size(); ++player){
				if ((*playerVector[player]).name == nextName){
					if (!(*playerVector[player]).onAuto){

						//if a non-automated player is successfully identified, log their results in their info file
						Player & quitter = *playerVector[player];
						quitterIndex = player;
						string outFile = nextName + ".txt";
						ofstream out(outFile);
						out << quitter << endl;
						out.close();
						playerExists = true;
					}
				}
			}

			//if the player exists and is not automated, remove them from the game
			if (playerExists){
				playerVector.erase(playerVector.begin() + quitterIndex);
			}

			//if the player does not exist or is automated
			else{
				std::cout << "There is not active player with that name or the chosen player is automated." << endl;
			}
		}
	}

	//logic regarding automated players leaving
	vector<string> leavingAI;

	//iterate through all players and for each automated player...
	for (unsigned int player = 0; player < roundResult.size(); ++player){
		if ((*roundResult[player]).onAuto){

			//generate a random integer from 1 to 100
			auto prob = rand() % 100 + 1;

			//if the player came in last, they leave with 90% probability
			if (player == 0){
				if (prob > 10){
					leavingAI.push_back((*roundResult[player]).name);
				}
			}

			//if the player came in first, they leave with 10% probability
			else if (player == roundResult.size() - 1){
				if (prob > 90){
					leavingAI.push_back((*roundResult[player]).name);
				}
			}

			//otherwise, they leave with 50% probability
			else {
				if (prob > 50){
					leavingAI.push_back((*roundResult[player]).name);
				}
			}
		}
	}

	//remove all identified AIs that are quitting
	for (unsigned int ai = 0; ai < leavingAI.size(); ++ai){
		removePlayer(leavingAI[ai]);
	}

	//handle all players that are joining
	bool playersJoined = false;

	//until players are done joining
	while (!playersJoined){
		std::cout << "Is there a player that would like to join the game? (Enter their name or 'no')" << endl;
		string nextName;
		std::cin >> nextName;

		//if players are done joining
		if (nextName == "no"){
			playersJoined = true;
		}

		//if another player is joining
		else {
			add_player(nextName);
		}
	}

	//shift the dealer to the next position
	++dealer;

	//if the dealer reaches the last player, send it back to the first player
	if (dealer >= playerVector.size()){
		dealer = 0;
	}
	string dummy;
	getline(cin, dummy);
	std::cout << endl;
	return COMPLETION_SUCCESS;
}

int FiveCardDraw::removePlayer(string name){
	for (unsigned int player = 0; player < playerVector.size(); ++player){
		if ((*playerVector[player]).name == name){
			playerVector.erase(playerVector.begin() + player);
		}
	}
	return COMPLETION_SUCCESS;
}

bool pokerRank(shared_ptr<Player> p1, shared_ptr<Player> p2){

	if (p1 == nullptr){
		return false;
	}

	else if (p2 == nullptr){
		return true;
	}

	return poker_rank((*p2).hand, (*p1).hand);
}

int operator<<(Deck & destination, Deck & start){
	int limit = start.size();
	for (int i = 0; i<limit; ++i){
		Card nextCard = start.stack[start.size() - 1];
		start.stack.pop_back();
		destination.addCard(nextCard);
	}
	return COMPLETION_SUCCESS;
}












