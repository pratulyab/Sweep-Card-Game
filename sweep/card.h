#ifndef CARD_H
#define CARD_H

#include <iostream>

using namespace std;

/*
	Card Class Assumptions:
	Ace Value = 1
	Card Values Between 1 and 13, inclusive
	4 Suites = {Spades,Hearts,Clubs,Diamonds}
 */

class Card{
	int value;
	char suite;

	static int isValidValue(int v){ //Returns 0 for invalid
		if(v >= 1 && v <= 13)
			return v;
		else
			return 0;
	}

	static char isValidSuite(char s){ //Returns '\0' for invalid
		if(s >= 'A' && s <= 'Z')
			return s;
		else if(s >= 'a' && s <= 'z')
			return (s - 'a' + 'A');
		else
			return  '\0';
	}

	public:

	Card() : value(0) , suite('\0') {}
	Card(int v, char s){
		v = isValidValue(v);
		s = isValidSuite(s);
		value = v;
		suite = s;
	}
	// Card(const Card &C) : value(C.value) , suite(C.suite) {} Redundant because no pointer vars (dyna alloc)

	int getValue() const{
		return value;
	}

	char getSuite() const{
		return suite;
	}

	// Redundant
	bool isValid(const Card &C) const{ 
		return (isValidValue(C.value) && isValidSuite(C.suite));
	}

	bool isSameValue(const Card &C) const{
		return value == C.value;
	}

	bool isSameSuite(const Card &C) const{
		return suite == C.suite;
	}
	
	bool operator==(const Card &C) const{
		return (value == C.value && suite == C.suite);
	}

	bool operator<(const Card &C) const{
		return (value < C.value);
	}

	friend ostream & operator<<(ostream &, const Card &);
};

ostream & operator<<(ostream &out, const Card &C){
	switch(C.value){
		case 1:
			cout << "A Of ";
			break;
		case 11:
			cout << "J Of ";
			break;
		case 12:
			cout << "Q Of ";
			break;
		case 13:
			cout << "K Of ";
			break;
		default:
			cout << C.value << " Of ";
			break;
	}
	switch(C.suite){
		case 'S':
			cout << "Spades\n";
			break;
		case 'H':
			cout << "Hearts\n";
			break;
		case 'C':
			cout << "Clubs\n";
			break;
		case 'D':
			cout << "Diamonds\n";
			break;
		default:
			cout << endl;
			//cout << "Aliens\n";
	}
	return out;
}

#endif
