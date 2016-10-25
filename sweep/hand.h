#ifndef HAND_H
#define HAND_H

#include "card.h"
#include "deck.h" // Including to avail SUITES, otherwise redefinition error
#include <vector>

/*
	Hand Class Assumptions:
	4 Suites => {S,H,C,D}
*/

//const char SUITES[4] = {'S','H','C','D'};

class Hand{
	vector<Card> pHand;
	
	int priority(char s){
		int len = sizeof(SUITES)/sizeof(SUITES[0]);

		for(int i=0 ; i<len ; i++){
			if (SUITES[i] == s){
				return len-i;
			}
		}
		return 0;
	}

	public:
	Hand() {}
	Hand(const vector<Card> &V){
		addCards(V);
	}
	Hand(const Card &C){
		addCards(C);
	}

	void addCards(const Card &C){
		pHand.push_back(C);
	}

	void addCards(const vector<Card> &V){
		vector<Card>::const_iterator it = V.begin();
		while(it != V.end()){
			pHand.push_back(*it);
			it++;
		}
	}

	Card removeCards(const Card &C){ //returns invalid(0) card if C not found
		vector<Card>::const_iterator it = pHand.begin();
		while(it != pHand.end()){
			if( (C == *it) ){
				Card temp = *it;
				pHand.erase(it);
				return temp;
			}
			it++;
		}
		return Card();
	}

	vector<Card> removeCards(const vector<Card> &V){
		vector<Card>::const_iterator it = V.begin();
		vector<Card> temp;
		while(it != V.end()){
			temp.push_back(removeCards(*it));
			it++;
		}
		return temp;
	}

	vector<Card> getKCards(int k){
		if(k<0 || size()-k < 0 || isEmpty())
			throw "Invalid number of cards requested to get from hand";
		vector<Card> temp;
		if(k>pHand.size())
			k = pHand.size();
		while(k--){
			temp.push_back(pHand.back());
			pHand.pop_back();
		}
		return temp;
	}

	int size() const{
		return pHand.size();
	}

	bool isEmpty() const{
		return pHand.empty();
	}

	const Card & operator[](int i) const{
		if(i<0 || i>=size()){
			throw "Out of bound indexing error occurred for hand on [] call";
		}
		return pHand[i];
	}

	Card operator[](int i){
		if(i<0 || i>=size()){
			throw "Out of bound indexing error occurred for hand on [] call";
		}
		return pHand[i];
	}

	void sortNormally(){
		for(int i=0 ; i<size()-1 ; i++){
			for(int j=i+1 ; j<size() ; j++){
				int pi = priority(pHand[i].getSuite());
				int pj = priority(pHand[j].getSuite());
				if(pj > pi){
					Card temp = pHand[j];
					pHand[j] = pHand[i];
					pHand[i] = temp;
				}
				else if(pi == pj){
					if(pHand[j].getValue() < pHand[i].getValue()){
						Card temp = pHand[j];
						pHand[j] = pHand[i];
						pHand[i] = temp;
					}
				}
			}
		}
	}

	void sortSuites(){
		for(int i=0 ; i<size()-1 ; i++){
			for(int j=i+1 ; j<size() ; j++){
				if(priority(pHand[j].getSuite()) > priority(pHand[i].getSuite())){
					Card temp = pHand[j];
					pHand[j] = pHand[i];
					pHand[i] = temp;
				}
			}
		}
	}

	void sortValues(){
		for(int i=0 ; i<size()-1 ; i++){
			for(int j=i+1 ; j<size() ; j++){
				if(pHand[j].getValue() < pHand[i].getValue()){
					Card temp = pHand[j];
					pHand[j] = pHand[i];
					pHand[i] = temp;
				}
			}
		}
	}

	int findCard(const Card &C) const{
		if(isEmpty())
			return -1;
		for(int i=0;i<pHand.size();i++){
			if(pHand[i] == C)
				return i;
		}
		return -1;
	}

	vector<int> findAllOfValue(int k) const{
		if(isEmpty() || (k<=0 || k>13))
			return vector<int>();
		vector<int> indices;
		for(int i=0; i<pHand.size(); i++){
			if(pHand[i].getValue() == k)
				indices.push_back(i);
		}
		return indices;
	}

	const vector<Card> & getHand() const{
		return pHand;
	}

	vector<Card> & getHand(){
		return pHand;
	}
	
	friend ostream & operator<<(ostream &,const Hand &);
};

ostream & operator<<(ostream &out, const Hand &H){
	for(int i=0 ; i<H.size(); i++){
		cout << H[i];
	}
	return out;
}

#endif
