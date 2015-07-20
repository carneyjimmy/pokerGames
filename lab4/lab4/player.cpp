//
//  player.cpp
//

#include "stdafx.h"
#include "player.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Player::Player(const char * inputName) : wins(0), losses(0), name(inputName), hand(Hand()), onAuto(false) {

	//set default wins, losses, chips, and hand
	wins = 0;
	losses = 0;
	chips = 20;	//new players star with 20 chips automatically
	cumulativeBet = 0;
	isFolded = false;
	hand = Hand();

	//create a pendingName to be tested for automation tag
	string pendingName = inputName;

	//set the onAuto member variable to true if automation tag is detected
	if (pendingName[pendingName.length() - 1] == '*'){
		onAuto = true;
		//remove the automation tag from the string to prepare for storing the name
		pendingName = pendingName.substr(0, pendingName.size() - 1);
	}
	name = pendingName;

	//open an ifstream to read from a player's info file
	ifstream ifs;
	string s(inputName);
	string fileName = s + ".txt";
	ifs.open(fileName);

	//if the file exists and was opened successfully, get info from it
	if (ifs.is_open()){
		string line;
		getline(ifs, line);
		istringstream iss(line);
		string word;
		//this first word is just the name, so we don't do anything with it since we already have the player's name
		iss >> word;
		iss >> word;
		wins = stoi(word);
		iss >> word;
		losses = stoi(word);
	}
}

ostream & operator<<(ostream & out, Player p){

	out << p.name << " " << p.wins << " " << p.losses << " ";

	return out;

}