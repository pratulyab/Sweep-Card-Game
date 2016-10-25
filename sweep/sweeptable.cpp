#include "sweeptable.h"

int main(){
	try{
	SweepTable T(2,1);
	cout << T;
	T.changeCurrentPlayer(1);
	T.distributeKCardsEachFromDeck(6);
	cout << "Current Player:\n"; T.printPlayer(T.getCurrentPlayerIndex() + 1);
	cout << "Next Player:\n"; T.printPlayer(T.nextPlayerIndex());
	cout << T.getDealerIndex();

	T.addCardsToTable(T.temp(4)); //created temp fxn just to test the class
	cout << T;

	vector<Card> V;
	V.push_back(T.removeCardsFromTable(Card(1,'D')));
	V.push_back(T.getCurrentPlayer().playCards(Card(9,'D')));
	T.createHouse(10,V);
	T.removeCardsFromTable(V);
	cout << "Current Player:\n"; T.printPlayer(T.getCurrentPlayerIndex() + 1);
	cout << T;
	
	T.addCardsToTable(Card(2,'S'));
	vector<vector<Card> > temp = T.createCombinationsOnlyFor(Card(12,'H'));
	cout << "Q of Hearts:\n";
	for(int i=0;i<temp.size();i++){
		for(int j=0;j<temp[i].size();j++){
			cout << temp[i][j].getValue() << temp[i][j].getSuite() << " ";
		}
		cout << endl;
	}
	cout << "Q of Clubs:\n";
	temp = T.createCombinationsOnlyFor(Card(12,'C'));
	for(int i=0;i<temp.size();i++){
		for(int j=0;j<temp[i].size();j++){
			cout << temp[i][j].getValue() << temp[i][j].getSuite() << " ";
		}
		cout << endl;
	}

	vector<pair<Card, vector<vector<Card> > > > P = T.createCombinations();
	vector<pair<Card, vector<vector<Card> > > >::iterator it = P.begin();
	while(it != P.end()){
		cout << "COMBINATIONS FOR " << it->first.getValue() << it->first.getSuite() << "-->\n";
		vector<vector<Card> >::iterator a = it->second.begin();
		while(a != it->second.end()){
			for(int i=0;i<a->size();i++){
				cout << (*a)[i].getValue() << (*a)[i].getSuite() << " ";
			}
			cout << endl;
			a++;
		}
		it++;
	}

	V.clear();
	V.push_back(Card(10,'H'));

//	cout << T.incrementHouse(10,Card(2,'H')) << "~~" << endl;
	cout << T.cementHouse(10,V) << endl;
	cout << T;

	vector<vector<Card> > comb = T.removeHouse(10);
	for(int i=0; i<comb.size(); i++){
		for(int j=0; j<comb[i].size();j++){
			cout << comb[i][j].getValue() << comb[i][j].getSuite() << " ";
		}
		cout << endl;
	}
	cout << T;

		
		V.clear();
		V.push_back(Card(1,'A'));
		V.push_back(Card(2,'A'));
		V.push_back(Card(9,'A'));
		vector<vector<pair<int, vector<vector<Card> > > > > combn;
		for(int i=0; i<V.size(); i++){
			combn.push_back(T.createHouseCombinations(V[i],vector<int>()));
		}
		cout << "--------------------\n";
		for(int i=0; i<V.size(); i++){
			cout << V[i];
			for(int j=0; j<combn[i].size(); j++){
				cout << "For House " << j+9 << " " << combn[i][j].first << endl;
				for(int k=0; k<combn[i][j].second.size(); k++){
					vector<Card>::iterator it = combn[i][j].second[k].begin();
					while(it != combn[i][j].second[k].end()){
						cout << it->getValue() << it->getSuite() << " ";
						it++;
					}
					cout << "| ";
				}
				cout << endl;
			}
			cout << "\n\n";
		}
	}
	catch(const char *msg){
		cout << msg;
		cout << "---" << endl;
	}
return 0;
}
