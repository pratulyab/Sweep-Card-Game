#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>
#include <iterator>
/*
	Deck Class Assumptions:
	Only 4 Suites = {Spades,Hearts,Clubs,Diamonds} in this decreasing order of priority
*/

const char SUITES[4] = {'S','H','C','D'};

class Deck{
	vector<Card> pack;
	
	int priority(char s){ // Returns 0 if suite is not present in SUITES
		int len = sizeof(SUITES)/sizeof(SUITES[0]);

		for(int i=0 ; i<len ; i++){
			if(s == SUITES[i])
				return len-i;
		}
		return 0;
	}

	void fillDeck() {
		int len = sizeof(SUITES)/sizeof(SUITES[0]);
		for(int i=0 ; i<len ; i++){
			for(int j=0 ; j<13 ; j++){
				pack.push_back( Card(j+1, SUITES[i]) );
			}
		}
	}

	public:

	Deck() { fillDeck(); }
	Deck(const Deck &D){
		for(int i=0 ; i<D.size() ; i++){
			pack.push_back(D[i]);
		}
	}

	const Card & operator[](int i) const{
		if(i<0 || i>=size()){
			throw "Out of bound indexing error occurred for deck on [] call";
		}
		return pack[i];
	}

	Card operator[](int i){
		if(i<0 || i>=size()){
			throw "Out of bound indexing error occurred for deck on [] call";
		}
		return pack[i];
	}

	void shuffle(){
	//	srand(time(0));
	//	random_shuffle(pack.begin(),pack.end());
		random_device rd;
		mt19937 g(rd());
		::shuffle(pack.begin(), pack.end(), g);
	}

	vector<Card> drawKCards(int k){ //Can't return by reference because temporary variable will be destroyed after execution of this function call
		if(k<0 || size()-k < 0 || isEmpty())
			throw "Invalid number of cards requested for drawing from the deck\n";
		vector<Card> temp;
		while(k--){
			temp.push_back(pack.back()); // Draws cards from the back i.e. top of the deck (i.e. as a stack) inorder to achieve Linear complexity
			pack.pop_back();
		}
		return temp;
	}

	int size() const{
		return pack.size();
	}

	bool isEmpty() const{
		return pack.empty();
	}

	void sort() { //Sorts in decreasing order of Suite Priority and in increasing order of values
		for(int i=0 ; i<size()-1 ; i++){
			for(int j=i+1 ; j<size() ; j++){
				int pi = priority(pack[i].getSuite());
				int pj = priority(pack[j].getSuite());
				if(pj > pi){
					Card temp = pack[j];
					pack[j] = pack[i];
					pack[i] = temp;
				}
				else if(pi == pj){
					if(pack[j].getValue() < pack[i].getValue()){
						Card temp = pack[j];
						pack[j] = pack[i];
						pack[i] = temp;
					}
				}
			}
		}
	}

	int find(Card C) const{
		if(isEmpty())
			return -1;
		for(int i=0; i<pack.size(); i++){
			if(pack[i] == C)
				return i;
		}
		return -1;
	}

	int findValue(int val) const{
		if(isEmpty())
			return -1;
		for(int i=0; i<pack.size(); i++){
			if(pack[i].getValue() == val)
				return i;
		}
		return-1;
	}

	int findValueLast(int val) const{
		if(isEmpty())
			return -1;
		for(int i=pack.size()-1; i>=0; i--){
			if(pack[i].getValue() == val)
				return i;
		}
		return -1;
	}

	void swapCards(int a,int b){
		if(isEmpty() || a<=0 || a>size() || b<=0 || b>size() || a==b)
			return;
		swap(pack[a-1],pack[b-1]);
	}

	friend ostream & operator<<(ostream &, const Deck &);
};

ostream & operator<<(ostream &out, const Deck &D){
	for(int i=0 ; i<D.size() ; i++){
		cout << D[i];
		
		if(i+1 % 13 == 0)
			cout << endl;
	}
	return out;
}

#endif
