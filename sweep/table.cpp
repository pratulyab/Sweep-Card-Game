#include "table.h"
//To start the game change current player's index from -1 to any valid one
int main(){
	vector<string> names;
	names.push_back("Pratulya");
	names.push_back("Bubna");
	Table T(4,1,names);
	cout << T;
	try{
		cout << T.getCurrentPlayer();
	}
	catch(const char *msg){
		cerr << msg;
	}
	cout << T.changeCurrentPlayer(1);
	T.distributeKCardsEachFromDeck(12);
	cout << T.changeCurrentPlayer(T.nextPlayerIndex());
	cout << T.changeCurrentPlayer(T.nextPlayerIndex());
	cout << T.changeCurrentPlayer(T.nextPlayerIndex());
	cout << T.changeCurrentPlayer(T.nextPlayerIndex());
	cout << T;
	T.addCardsToTable(T.getCurrentPlayer().playKCards(4));
	cout << T;
	return 0;
}
