#ifndef SWEEP_H
#define SWEEP_H

#include "sweeptable.h"
#include <limits>

class Sweep {
	SweepTable table;
	int validMoves;

	public:

	Sweep(int nop=4, int dealer=1) : table(nop,dealer) , validMoves(0) {}
	Sweep(int nop, int dealer, const vector<string> &names) : table(nop,dealer,names) , validMoves(0){}

	bool pick(int housevalue = 0) {
		if(table.isFloorEmpty()){
			cout << "Floor is empty. Cannot pick from an empty floor, can you?\n";
			return false;
		}
		
		vector<Card> pHand = table.getCurrentPlayer().getPlayerHand();
		if(pHand.empty()){
			cout << "Player doesn't have any cards in his hand\n";
			return false;
		}
		vector<vector<vector<Card> > > combns;
		
		if(housevalue){
			vector<Card>::iterator it = pHand.begin();
			while(it != pHand.end()){
//				cout << "HI " << *it << pHand.size() << endl;
				if(it->getValue() == housevalue){
					combns.push_back(table.createCombinationsOnlyFor(*it));
					//adding permanent house combinations.. Although, there won't be any as housevalue will be considered only for first chance
					if(!table.tableHouses.empty()){
						int index = table.findHouse(housevalue);
						if(index == -1 || !table.tableHouses[index].permanent()){ //because temporary houses have already been considered in the create fxn
							it++;
							continue;
						}
						vector<Card> V;
						V.push_back(Card(housevalue,'\0'));
						combns.back().push_back(V);
					}
					it++;
					continue;
				}
				//else deleting the card i.e. if not housevalue value
				it = pHand.erase(it);
			}
			if(pHand.empty()){
				cout << "You don't possess a card of value housevalue to pick an equivalent card\n";
				return false;
			}
		}
		else{
			for(int i=0; i<pHand.size(); i++){
				combns.push_back(table.createCombinationsOnlyFor(pHand[i]));
				
				//Adding permanent house combinations
				if(pHand[i].getValue() >= 9){
					int index = table.findHouse(pHand[i].getValue());
					if(index == -1 || !table.tableHouses[index].permanent())
						continue;
					vector<Card> V;
					V.push_back(Card(pHand[i].getValue(), '\0'));
					combns.back().push_back(V);
				}
			}
		}
		int removed = 0;
		for(int i=0; i<combns.size(); i++){
			if(combns[i].empty()){
				pHand.erase(pHand.begin() + i - removed);
				combns.erase(combns.begin() + i - removed);
				removed += 1;
			}
		}

		if(combns.empty()){
			cout << "No combinations found to pick." << endl;
			return false;
		}

		cout << "-- OPTIONS --" << endl;
		for(int i=0; i<pHand.size(); i++){
			cout << i+1 << ") Using " << pHand[i].getValue() << pHand[i].getSuite() << ", pick: " << endl;
			for(int j=0; j<combns[i].size(); j++){
				vector<Card> &temp = combns[i][j];
				cout << "\t" << j+1 << ") ";
				for(int k=0; k<temp.size(); k++){
					cout << temp[k].getValue() << temp[k].getSuite();
					if(temp[k].getSuite() == '\0'){
						cout << table.tableHouses[table.findHouse(temp[k].getValue())];
					}
					cout << " ";
				}
				cout << endl;
			}
			cout << "-----" << endl;
		}
		cout << "Enter 2 Numbers [Main, Nested]: " << endl;
		int a,b;
		while(1){
			cin.clear();
			//cin.ignore(INT_MAX);
			cin >> a;
			if(a==-1)
				return false;
			if(a<=0 || a>pHand.size()){
				cout << "Invalid 'Main' input." << endl;
				continue;
			}
			break;
		}
		while(1){
			cin.clear();
			//cin.ignore(INT_MAX);
			cin >> b;
			if(b==-1)
				return false;
			if(b<=0 || b>combns[a-1].size()){
			cout << "Invalid 'Nested' input." << endl;
			continue;
			}
			break;
		}
		
		Card C = table.getCurrentPlayer().playCards(pHand[a-1]);
		vector<Card> &temp = combns[a-1][b-1];
		vector<vector<Card> > perm;
		vector<Card> vec;

		for(int i=0; i<temp.size(); i++){
			if(temp[i].getSuite() == '\0'){
				table.removeHouse(temp[i].getValue());
				continue;
			}
			table.removeCardsFromTable(temp[i]);
		}

		/*
			Note: When a user decides to pick a value X, then he must pick all the non-overlapping combinations of value X.
			NOT YET IMPLEMENTED :(
		*/

		//update score using C and perm/vec
		//......
		//......
		return true;
	}

	bool thro(int housevalue = 0) {

		vector<vector<vector<Card> > > combns;
		vector<Card> pHand = table.getCurrentPlayer().getPlayerHand();
		if(housevalue){
			vector<Card>::iterator it = pHand.begin();
			while(it != pHand.end()){
				if(it->getValue() == housevalue){
					combns.push_back(table.createCombinationsOnlyFor(*it));
					int index = table.findHouse(it->getValue());
					if(index == -1 || !table.tableHouses[index].permanent()){
						it++;
						continue;
					}
					vector<Card> V;
					V.push_back(Card(it->getValue(), '\0'));
					combns.back().push_back(V);
					it++;
					continue;
				}
				it = pHand.erase(it);
			}

			if(pHand.empty()){
				cout << "You don't possess a card of value housevalue to pick an equivalent card\n";
				return false;
			}
		}
		else{
			for(int i=0; i<pHand.size(); i++){
				combns.push_back(table.createCombinationsOnlyFor(pHand[i]));
				
				//Adding permanent house combinations
				if(pHand[i].getValue() >= 9){
					int index = table.findHouse(pHand[i].getValue());
					if(index == -1 || !table.tableHouses[index].permanent())
						continue;
					vector<Card> V;
					V.push_back(Card(pHand[i].getValue(), '\0'));
					combns.back().push_back(V);
				}
			}
		}

		vector<Card> throwable;
		for(int i=0; i<pHand.size(); i++){
			if(combns[i].size() == 0)
				throwable.push_back(pHand[i]);
		}

		if(throwable.size() == 0){
			cout << "Can't throw because you'll have to pick (Maybe the card of that value is present on the floor)." << endl;
			return false;
		}

		cout << "-- OPTIONS --" << endl;
		for(int i=0; i<throwable.size(); i++){
			cout << i+1 << ") Throw " << throwable[i].getValue() << throwable[i].getSuite() << endl;
		}
		cout << "Enter: ";
		int inp;
		while(1){
			cin.clear();
			//cin.ignore(INT_MAX);
			cin >> inp;
			if(inp==-1)
				return false;
			if(inp<=0 || inp>throwable.size()){
				cout << "Invalid input." << endl;
				continue;
			}
			break;
		}
		table.addCardsToTable(table.getCurrentPlayer().playCards(throwable[inp-1]));
		return true;
	}

	bool combine(int housevalue = 0) {
		// Note: Need to add constraint to check if available combination is disguised as cover..
		if(table.isFloorEmpty()){
			cout << "Floor is empty." << endl;
			return false;
		}

		if(table.isHandEmpty()){
			if(housevalue){
				int index = table.findHouse(housevalue);
				for(int i=0; i<table.tableHouses.size(); i++){
					if(i==index)
						continue;
					if((table.tableHouses[i].getValue() < housevalue) && (!table.tableHouses[i].permanent()) && (table.tableHouses[i].getCreator()!=table.getCurrentPlayerIndex()))
						break;
					if(i+1 == table.tableHouses.size()){
						cout << "You cannot combine as there are no loose cards and neither any temporary houses that aren't created by you.\n";
						return false;
					}
				}
			}

			// Hand empty and all houses permanent or temporary houses but created by the current player
			else{
				for(int i=0; i<table.tableHouses.size(); i++){
					if(!table.tableHouses[i].permanent() && (table.tableHouses[i].getCreator() != table.getCurrentPlayerIndex())){
						break;
					}
					if(i+1 == table.tableHouses.size()){
						cout << "No loose cards. All houses (if any) are permanent. Invalid." << endl;
						return false;
					}
				}
			}
		}
		
		vector<Card> pHand = table.getCurrentPlayer().getPlayerHand();

		//checking for cards of value >=9 for combine opertion
		// if user doesn't have any house value card, then Combine isn't possible
		vector<int> houseValues;
		if(housevalue){
			for(int i=0; i<pHand.size(); i++){
				if(pHand[i].getValue() == housevalue){
					houseValues.push_back(housevalue);
					break;
				}
			}
			if(houseValues.empty()){
				cout << "You don't have card of value housevalue\n";
				return false;
			}
		}
		else{
			for(int i=9; i<14; i++)
				for(int j=0; j<pHand.size(); j++)
					if(pHand[j].getValue() == i){
						houseValues.push_back(i);
						break;
					}
			if(houseValues.empty()){
				cout << "You don't have any card of value >= 9. Invalid." << endl;
				return false;
			}
		}

		vector<vector<pair<int, vector<vector<Card> > > > > combn; //playercard<housevalue<pair<value, combinations> > >
		for(int i=0; i<pHand.size(); i++){
			combn.push_back(table.createHouseCombinations(pHand[i], houseValues));
		}
		
		for(int i=0; i<combn.size(); i++){
			bool breaking = false;
			for(int j=0; j<combn[i].size(); j++){
				if(!combn[i][j].second.empty()){
					breaking = true;
					break;
				}
			}
			if(breaking)
				break;
			if(i+1 == combn.size()){
				cout << "Combine is not possible." << endl;
				return false;
			}
		}
		
		cout << "-- OPTIONS --" << endl;
		for(int i=0; i<houseValues.size(); i++){
			cout << i+1 << ") Combine to form house of " << houseValues[i] << endl;
			for(int j=0; j<combn.size(); j++){
				vector<vector<Card> > &temp = combn[j][i].second;
				if(temp.empty()) //no combinations for house value using the card combns[j][i].first
					continue;
				cout << "\t" << j+1 << ") Using " << pHand[j].getValue() << pHand[j].getSuite() << endl;
				for(int k=0; k<temp.size(); k++){
					vector<Card>::iterator it = temp[k].begin();
					cout << "\t\t" << k+1 << ") ";
					while(it != temp[k].end()){
						cout << it->getValue() << it->getSuite() << " ";
						it++;
					}
					cout << endl;
				}
			}
		}

		cout << "Enter [HouseValueNo., YourCardNo., Comb.No]: ";
		int a,b,c;
		while(1){
			cin.clear();
			//cin.ignore(INT_MAX);
			cin >> a >> b;
			if(a==-1 || b==-1)
				return false;
			if(a<=0 || a>houseValues.size()){
				cout << "Invalid HouseValue Input.\n";
				continue;
			}
			if(b<=0 || b>pHand.size()){
				cout << "Invalid YourCardNo. Input.\n";
				continue;
			}
			if(combn[b-1][a-1].second.empty()){
				cout << "No combinations under current house value using the chosen card. Try entering different inputs." << endl;
				continue;
			}
			break;
		}

		vector<vector<Card> > &temp = combn[b-1][a-1].second;

		while(1){
			cin.clear();
			//cin.ignore(INT_MAX);
			cin >> c;
			if(c==-1)
				return false;
			if(c<=0 || c>temp.size()){
				cout << "Invalid Comb.No Input.\n";
				continue;
			}
			break;
		}

		//if a temporary house is included in the chosen combination, then we have to remove that temporary house, transfer its combinations to the new value house
		//final contains the combinations of the previous house
		vector<vector<Card> > final;

		for(int i=0; i<temp[c-1].size(); i++)
			if(temp[c-1][i].getSuite() == '\0')
				final = table.removeHouse(temp[c-1][i].getValue());
		//now adding the rest of the combination cards required in addition to the prev house value card to each of the combination vector of prev
		if(!final.empty()){
			for(int i=0; i<temp[c-1].size(); i++){
				if(temp[c-1][i].getSuite() == '\0')
					continue;
				vector<vector<Card> >::iterator it = final.begin();
				while(it != final.end()){
					it->push_back(temp[c-1][i]);
					it++;
				}
			}
		}
		else{
			//i.e. no temporary house used in the combination
			//therefore, need to add the normal combination
			final.push_back(temp[c-1]);
		}

		try{
			if(table.findHouse(combn[b-1][a-1].first) == -1){
				if(!table.createHouse(combn[b-1][a-1].first, final[0])){
					cout << "Error occurred while creating house.\n";
					return false;
				}
			}
			else{
				if(!table.cementHouse(combn[b-1][a-1].first, final[0])){
					cout << "Error occurred while cementing house.\n";
					return false;
				}
			}
		}
		catch(const char *msg){
			cerr << msg;
			throw;
		}

		if(final.size() > 1){
			for(int i=1; i<final.size(); i++)
				table.cementHouse(combn[b-1][a-1].first, final[i]);
		}
		
		//Removing the cards now
		//We've already removed the temporary house if it was included in the chosen combination
		Card pCard = table.getCurrentPlayer().playCards(pHand[b-1]);
		for(int i=0; i<temp[c-1].size(); i++){
			if((temp[c-1][i].getSuite() == '\0') || (temp[c-1][i] == pCard))
				continue;
			table.removeCardsFromTable(temp[c-1][i]);
		}
		return true;
	}

	bool cover(int housevalue = 0) {
		if(table.tableHouses.empty()){
			cout << "No houses on floor." << endl;
			return false;
		}
		
		vector<pair<int, vector<int> > > V; //value, vector<indices of card of that value in player's hand

		if(housevalue){
			V.push_back(make_pair(housevalue, table.getCurrentPlayer().hasCardsOfValueAll(housevalue)));
			if(V[0].second.size() < 2)
				V.erase(V.begin());
		}
		else{
			//Gathering all indices of house cards that the player has
			for(int i=9; i<14; i++)
				V.push_back(make_pair(i, table.getCurrentPlayer().hasCardsOfValueAll(i)));

			//Considering only the cards whose count>=2 in players hand
			vector<pair<int, vector<int> > >::iterator it = V.begin();
			while(it != V.end()){
				if(it->second.size() < 2)
					it = V.erase(it);
				else
					it++;
			}
		}
		if(V.empty()){
			cout << "Not valid cards/number of cards." << endl;
			return false;
		}

		//checking whether there are any houses present that can be covered
		vector<pair<int, vector<int> > >::iterator it = V.begin();
		vector<int> indices; //will store indices of HC
		while(it != V.end()){
			bool erased = false;
			for(int i=0; i<table.tableHouses.size(); i++){
				if(table.tableHouses[i].getValue() == it->first){
					indices.push_back(i);
					break;
				}
				if(it+1 == V.end()){
					it = V.erase(it);
					erased = true;
				}
			}
			if(!erased)
				it++;
		}

		if(V.empty()){
			cout << "No houses yet created on the floor that match your potential house cards." << endl;
			return false;
		}

		//Now, asking to choose among the contenders
		cout << "-- OPTIONS --" << endl;
		int k=0;
		for(int i=0;i<V.size();i++){
			cout << i+1 << ") Cover House Of " << V[i].first << endl;
			cout << "\t";
			table.tableHouses[indices[k++]].printCombinations();
		}
		cout << "Enter: ";
		int inp;
		while(1){
			cin.clear();
			//cin.ignore(INT_MAX);
			cin >> inp;
			if(inp <=0 || inp > V.size()){
				cout << "Invalid input\n";
				continue;
			}
			break;
		}

		vector<Card> pHand = table.getCurrentPlayer().getPlayerHand();

		// Asking the card to be used to cover the house
		cout << "using, \n";
		int val = V[inp-1].first;
		vector<int> &temp = V[inp-1].second;
		for(int i=0; i<temp.size(); i++){
			cout << i+1 << ") " << pHand[temp[i]].getValue() << pHand[temp[i]].getSuite() << endl;
		}
		cout << "Enter: ";
		while(1){
			cin.clear();
			//cin.ignore(INT_MAX);
			cin >> inp;
			if(inp==-1)
				return false;
			if(inp <= 0 || inp > temp.size()){
				cout << "Invalid input\n";
				continue;
			}
			break;
		}

		vector<Card> vec;
		vec.push_back(pHand[temp[inp-1]]);
		return table.cementHouse(val, table.getCurrentPlayer().playCards(vec));
	}

	int start(){
		//changing the current player to the one next to dealer
		int d = table.getDealerIndex();
		if(d == table.numberOfPlayers()){
			d = 0;
		}
		table.changeCurrentPlayer(d+1);

		//A house value card in bottom 4 because we're using pop_back
		for(int i=9; i<14; i++){
			int index = table.tableDeck.findValueLast(i);
			if(index>=table.tableDeck.size()-4)
				break;
			if(i+1 == 14){
				vector<int> V;
				V.push_back(9);V.push_back(10);V.push_back(11);V.push_back(12);V.push_back(13);
				srand(time(0));
				random_shuffle(V.begin(),V.end());
				table.tableDeck.swapCards(table.tableDeck.size(),table.tableDeck.findValue(V[0])+1);
			}
		}

		//distribute 4 cards to each player
		table.distributeKCardsEachFromDeck(4);

		//Adding 4 cards to the table
		table.addCardsToTable(table.tableDeck.drawKCards(4));

		displayCurrentPlayer();

		cout << "Enter Value Of The House You'd Like To Build: ";
		int hv;
		while(1){
			cin.clear();
			//cin.ignore(INT_MAX);
			cin >> hv;
			if(hv<9 || hv>13){
				cout << "Invalid House Value.\n";
				continue;
			}
			vector<Card> pHand = table.getCurrentPlayer().getPlayerHand();
			bool has = false;
			for(int i=0; i<pHand.size(); i++)
				if(pHand[i].getValue() == hv){
					has = true;
					break;
				}
			if(!has){
				cout << "You don't possess a card of house value you've provided.\n";
				continue;
			}
			break;
		}
		displayTable();
		return hv;
	}

	void proceed(){
		validMoves++;
		if(validMoves == 1){
			table.distributeKCardsEachFromDeck(8);
		}
		table.changeCurrentPlayer(table.nextPlayerIndex());
		if(validMoves == 24 && table.numberOfPlayers() == 2) {
			table.distributeKCardsEachFromDeck(8);
		}
		displayTable();
		table.getCurrentPlayer().sortHandValues();
		displayCurrentPlayer();
		if(validMoves == 48){
			//add all points on floor to the person who picked up last
			cout << "-------GAME OVER-------" << endl;
			//declare WINNER score-wise
		}
	}

	void sweeped(){
		if(!table.isFloorEmpty() || validMoves==47){
			return;
		}
		int prev = table.getCurrentPlayerIndex() - 1;
		if(prev == -1)
			prev = table.numberOfPlayers() - 1;
		cout << "-=-=-=-=-=-=-= " << table.players[prev].getName() << " has been SWEEPED by " << table.getCurrentPlayer().getName() << " -=-=-=-=-=-=-=" << endl;
		table.getCurrentPlayer().updateScore(50);
	}

	int calculateScore(vector<Card> V){
		vector<Card>::iterator it;
		int sum = 0;
		for(it=V.begin(); it!=V.end(); it++){
			char suite = it->getSuite();
			int value = it->getValue();
			switch(suite){
				case 'S':
					sum += value;
					break;
				case 'H':
					if(value == 1)
						sum += 1;
					break;
				case 'C':
					if(value == 1)
						sum += 1;
					break;
				case 'D':
					if(value == 1)
						sum += 1;
					else if(value ==10)
						sum += 6;
					break;
			}
		}
		return sum;
	}

	void displayTable() const{
		cout << "\n---- TABLE ----\n";
		cout << "Number Of Players: " << table.numberOfPlayers() << endl;
		cout << "Current Player: " << table.getCurrentPlayer().getName() << endl;
		cout << "Valid Moves Played: " << validMoves << endl;
		cout << "\nLoose Cards: ";
		for(int i=0; i<table.tableHand.size(); i++){
			cout << table.tableHand[i].getValue() << table.tableHand[i].getSuite() << " ";
		}
		cout << "\nTable Houses: " << endl;
		for(int i=0; i<table.tableHouses.size(); i++){
			table.tableHouses[i].printCombinations();
			cout << "\t\tCreator: " << table.getKthPlayer(table.tableHouses[i].getCreator()+1).getName() << endl;
		}
		cout << "-----------------\n";
	}

	void displayCurrentPlayer() const{
		if(table.currentPlayer == -1)
			return;
		const Player &P = table.getCurrentPlayer();
		cout << "\t--- " << P.getName() << " [" << P.getScore() << "]" << " ---" << endl;
		vector<Card> pHand = P.getPlayerHand();
		cout << "\t";
		for(int i=0; i<pHand.size(); i++)
			cout << pHand[i].getValue() << pHand[i].getSuite() << " ";
		cout << endl;
	}
};

#endif
