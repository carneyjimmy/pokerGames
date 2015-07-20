//
//  deck.cpp
//  Lab4
//

#include "stdafx.h"
#include "deck.h"
#include "Lab4.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <iterator>

using namespace std;

Deck::Deck(){

}

Deck::Deck(const char * cardFile){
	int loadResult = load(cardFile);
	if (loadResult == FILE_OPEN_ERROR){
		throw FILE_OPEN_ERROR;
	}
}

int Deck::addCard(Card & c){
	stack.push_back(c);
	return 0;
}

int Deck::load(const char * cardFile){ //load method parses card definition strings from an input file

	//Create an input file stream and try to open the input file
	ifstream ifs;
	ifs.open(cardFile);
	if (ifs.is_open()){
		string line, word;
		while (getline(ifs, line)){			//Begin reading line by line until the end of the file
			istringstream cardReader(line);
			while (cardReader >> word && word[word.size() - 1] != '/' && word[word.size() - 2] != '/'){     //Read each line word by word until the end of the line
				//Begin By Parsing 10s
				//If no valid suit is found or if the rank is not 10, the card is not 'push_back'ed to the vector.
				if (word.size() == 3){
					//Check for proper numbering (to eliminate invalid card strings)
					if (word[0] == '1' && word[1] == '0'){
						Card nextCard;
						nextCard.rank = Card::ten;
						checkSuit(word, nextCard);	//Check for valid suit
					}
					else {
						cout << "WARNING: Invalid Card Definition Caught and Skipped" << endl;
					}
				}

				//Continue parsing all cards that are not 10s
				else if (word.size() == 2){
					Card nextCard;

					//The following strings include all valid chars for rank and suit respectively
					const char * validRanks = "AaKkQqJj98765432";
					const char * validSuits = "SsDdCcHh";

					//Check to see if each char in the card string matches a char in the validity strings above
					//strchr function is used to perform this check, passing the conditional if both strchr functions return a value other than NULL
					if ((strchr(validRanks, word[0]) != NULL) && (strchr(validSuits, word[1]) != NULL)){
						nextCard.rank = assignRank(word);	//assign valid card's rank
						nextCard.suit = assignSuit(word);	//assign valid card's suit	
						(*this).stack.push_back(nextCard);	//Add new valid card to the vector of cards
					}
					else {
						cout << "WARNING: Invalid Card Definition Caught and Skipped" << endl;
					}
				}
				else {
					cout << "WARNING: Invalid Card Definition Caught and Skipped" << endl;
				}

			}
		}
	}

	//Print error statement and return error code 2 if file cannot be opened properly or cannot be found.
	else {
		cerr << "The file was unable to be opened or does not exist (Error Code 1)" << endl;
		return FILE_OPEN_ERROR;
	}

	return COMPLETION_SUCCESS;
}

void Deck::checkSuit(string & word, Card & nextCard){
	if (word[2] == 'C' || word[2] == 'c'){
		nextCard.suit = Card::clubs;
		(*this).stack.push_back(nextCard);
	}
	else if (word[2] == 'D' || word[2] == 'd'){
		nextCard.suit = Card::diamonds;
		(*this).stack.push_back(nextCard);
	}
	else if (word[2] == 'H' || word[2] == 'h'){
		nextCard.suit = Card::hearts;
		(*this).stack.push_back(nextCard);
	}
	else if (word[2] == 'S' || word[2] == 's'){
		nextCard.suit = Card::spades;
		(*this).stack.push_back(nextCard);
	}
	else {
		cout << "WARNING: Invalid Card Definition Caught and Skipped" << endl;
	}
}

Card::Rank Deck::assignRank(string & word){
	if (word[0] == 'A' || word[0] == 'a'){
		return Card::ace;
	}
	else if (word[0] == 'K' || word[0] == 'k'){
		return Card::king;
	}
	else if (word[0] == 'Q' || word[0] == 'q'){
		return Card::queen;
	}
	else if (word[0] == 'J' || word[0] == 'j'){
		return Card::jack;
	}
	else{
		//String stream used to convert the first char of the card string to an integer.  Any cards that fail the first 4 if statements will have a numeric rank (2-9).
		istringstream wordReader(word);
		int x;
		wordReader >> x;
		Card::Rank nextRank = (Card::Rank)x;  //Notation used to convert from integer to enumeration.
		wordReader.clear();
		return nextRank;
	}
}

Card::Suit Deck::assignSuit(string & word){
	if (word[1] == 'C' || word[1] == 'c'){
		return Card::clubs;
	}
	else if (word[1] == 'D' || word[1] == 'd'){
		return Card::diamonds;
	}
	else if (word[1] == 'H' || word[1] == 'h'){
		return Card::hearts;
	}
	else if (word[1] == 'S' || word[1] == 's'){
		return Card::spades;
	}
	else{
		return Card::spades;	//defualt suit
	}
}

int Deck::shuffle(){

	random_device rd;
	std::mt19937 g(rd());
	std::shuffle((*this).stack.begin(), (*this).stack.end(), g);

	return COMPLETION_SUCCESS;

}

int Deck::size() const{

	int size = (*this).stack.size();
	return size;
}

int operator<<(ostream & out, const Deck & deck){

	//Create two arrays containing string representations of the Rank and Suit enumerations to convert integers for printing and reading purposes.
	const string rankConverter[] = { "zero", "one", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
	const string suitConverter[] = { "C", "D", "H", "S" };

	//For loop iterates through entire vector, printing the converted rank and suit of each Card struct as a readable string.
	//If an invalid card somehow makes it through parsing undetected, it will be noticed here with error code 3 being tripped.
	for (unsigned int i = 0; i < deck.stack.size(); ++i){
		if (deck.stack[i].rank > 1 && deck.stack[i].rank < 15 && deck.stack[i].suit > -1 && deck.stack[i].suit < 4){
			cout << rankConverter[deck.stack[i].rank] << suitConverter[deck.stack[i].suit] << endl;
		}
		else {
			cerr << "The program has encountered a card with an invalid suit or rank (Error Code 2)" << endl;
			return INVALID_CARD_DEFINITION;
		}
	}

	return COMPLETION_SUCCESS;

}
