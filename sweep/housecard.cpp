#include "housecard.h"
int main(){
	vector<Card> V;
	V.push_back(Card(9,'S'));
	V.push_back(Card(3,'D'));
	try{
		HouseCard H(9,V,1);
	cout << H;
	V.clear();
	V.push_back(Card(8,'C'));
	V.push_back(Card(4,'S'));
	H.addCombination(V);
	cout << H;
	}
	catch(const char *msg){
		cerr << msg;
		exit(0);
	}
	return 0;
}
