//
//  card.cpp
//  Lab4
//

#include "stdafx.h"
#include "card.h"
#include "Lab4.h"
#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;

bool Card::operator< (const Card & card2) const{

	//Check case where cards have same rank
	if ((*this).rank == card2.rank){
		if ((*this).suit < card2.suit){
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if ((*this).rank < card2.rank){
			return true;
		}
		else {
			return false;
		}
	}
}
