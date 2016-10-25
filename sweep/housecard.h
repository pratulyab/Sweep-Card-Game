#ifndef HOUSE_CARD_H
#define HOUSE_CARD_H

#include "card.h"
#include <vector>

class HouseCard{
	int value;
	vector<vector<Card> > combinations;
	int created_by; //player vector index
	bool isPermanent;


	static int combinationsValidator(const vector<vector<Card> > &V){ //returns 0 incase invalid
		int sum = 0, prev = 0;
		vector<vector<Card> >::const_iterator it = V.begin();
		
		while(it != V.end()){
			if(!it->size())
				continue;

			sum = 0;
			
			vector<Card>::const_iterator i = it->begin();
			while(i != it->end()){
				sum += i->getValue();
				i++;
			}

			if(sum == 0 || (prev!=0 && prev!=sum))
				return 0;

			prev = sum;

			it++;
		}
		return sum;
	}

	static int valueValidator(int sum){ //returns 0 incase invalid
		if(sum >= 9 && sum <= 13)
			return sum;
		else
			return 0;
	}

	static int valueValidator(const vector<Card> &V){
		int sum = 0;
		vector<Card>::const_iterator it = V.begin();
		while(it != V.end()){
			sum += it->getValue();
			it++;
		}
		return valueValidator(sum);
	}

	public:

	// Assuming that correct player index is provided. If not, error will be raised while accessing player, but not while creating house.

	HouseCard(const vector<vector<Card> > &V, int cb){

		if( !(value = combinationsValidator(V)) ){
			throw "The combinations provided to create house are not same. Please provide same combinations.\n";
		}

		/*
		for(int i=0; i<V.size() ; i++){
			if(V[i].size())
				combinations.push_back(V[i]);
		}*/
		combinations = V;
		created_by = cb;
		isPermanent = true;
	}

	HouseCard(const vector<Card> &V, int cb) : isPermanent(false) {
		if( !(value = valueValidator(V)) )
			throw "Invalid value provided to create house.\n";

		combinations.push_back(V);
		created_by = cb;
	}

	HouseCard(int val, vector<Card> &V, int cb) : isPermanent(false) {
		if( !(value = valueValidator(V)) )
			throw "Invalid value of combination provided to create house.\n";
		if(val != value)
			throw "Value of combination does not match the intended value provided for house creation.\n";
		combinations.push_back(V);
		created_by = cb;
	}

	void addCombination(const vector<Card> &V){
		int val = valueValidator(V);
		if(val != value)
			throw "Value of combination does not match the intended value provided for house creation.\n";
		combinations.push_back(V);
		if(combinations.size() > 1)
			isPermanent = true;
	}

	void changeCreator(int c){
		created_by = c;
	}

	vector<vector<Card> > getCombinations() const{
		return combinations;
	}

	int getValue() const{
		return value;
	}

	void printCombinations() const{
		cout << value << " => ";
		for(int i=0;i<combinations.size();i++){
			for(int j=0; j<combinations[i].size(); j++)
				cout << combinations[i][j].getValue() << combinations[i][j].getSuite() << " ";
			cout << "| ";
		}
		cout << endl;
	}

	int getCreator() const{
		return created_by;
	}

	bool permanent() const{
		return isPermanent;
	}

	friend class SweepTable;
	friend ostream & operator<<(ostream &, const HouseCard &);
};

ostream & operator<<(ostream &out, const HouseCard &HC){
	out << "House Of " << HC.value << "\n{\n";
	vector<vector<Card> >::const_iterator it = HC.combinations.begin();
	while(it != HC.combinations.end()){
		out << "[";
		for(int i=0; i<it->size(); i++){
			out << (*it)[i].getValue() << (*it)[i].getSuite();
			if(i+1!=it->size())
				out << ", ";
		}
		out << "]" << endl;
		it++;
	}
	out << "}\n";
	return out;
}

#endif
