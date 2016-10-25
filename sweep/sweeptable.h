#ifndef SWEEP_TABLE_H
#define SWEEP_TABLE_H

#include "table.h"
#include "housecard.h"
/*
	There isn't a need for a separate SweepTableCard class because we can handle it using the normal Hand..
	Also, it isn't necessary to keep a vector for each and every card because we'll be requiring the combination vector only during the Pick and Throw operations 
	To achieve this, we need to define two functions which would return vector[Card] of combinations: 
		1) Creating combinations for the existing cards in market
		2) Creating combinations including a Card passed as an argument

	However, we do need to create a HouseCard class as our houses are stack of combinations of 2 or more cards.
	After combining cards to form a house, we need to delete the cards from normal cards Hand
	Thus, when printing the table: print the hand normally. But, display all the combinations of the housecard stored in its combination vector
*/

class SweepTable : public Table{
	
	/*
	   if(!isPermanent && created_by!=current_player){
			house value can be incremented, if desired;
			update combinations; (since house isn't permanent yet, there will be only one combination vector in 'combinations'. Thus, add the new card to it)
			update created_by;
		}
	*/
	
	/*
		Inherits:
		vector<Player> players;
		int currentPlayer;
		int dealerIndex;
		Deck tableDeck;
		Hand tableHand;
	*/

	vector<HouseCard> tableHouses;

	static void findCombinationsHelper(vector<Card> temp, int index, int size, vector<Card> aux, int target, vector<vector<Card> > &combination){
		if(target <= 0)
			return;
		if(index >= size){
			if(aux.empty())
				return;
			int sum = 0;
			for(int i=0; i<aux.size(); i++)
				sum += aux[i].getValue();
			if(sum == target){
				combination.push_back(aux);
			}
			return;
		}
		findCombinationsHelper(temp,index+1,size,aux,target,combination);
		aux.push_back(temp[index]);
		findCombinationsHelper(temp,index+1,size,aux,target,combination);
	}

	static void findCombinations(vector<Card> CV, vector<pair<Card, vector<vector<Card> > > > &combn){
		sort(CV.begin(),CV.end());
		
		for(int i=0; i<CV.size(); i++){
			vector<Card> temp;
			int target_sum;

			for(int j=0; j<CV.size(); j++){
				if(i==j){
					target_sum = CV[i].getValue();
					continue;
				}
				temp.push_back(CV[j]);
			}
			vector<Card> auxiliary;
			vector<vector<Card> > combination;
			findCombinationsHelper(temp,0,temp.size(),auxiliary,target_sum,combination);
			combn.push_back( make_pair(CV[i],combination) );
		}
	}

	static int playersValidator(int n){
		if(n==2 || n==4)
			return n;
		else
			throw "Seep can be played with either 2 or 4 players only.\n";
	}

	static int houseValueValidator(int val){ //returns -1 incase invalid
		if(val>=9 && val<=13)
			return val;
		else
			return -1;
	}

	static int combinationValueValidator(int val, const vector<Card> &V){
		int sum = 0;
		for(int i=0;i<V.size();i++){
			sum += V[i].getValue();
		}
		if(val!=0 && sum!=0 && sum==val)
			return sum;
		else
			return -1;
	}

	public:

	SweepTable(int nop, int dealer) : Table(playersValidator(nop), dealer){}
	SweepTable(int nop, int dealer, const vector<string> &names) : Table(playersValidator(nop), dealer, names) {}

	vector<pair<Card, vector<vector<Card> > > > createCombinations() const{ //only for cards on table
		if(isFloorEmpty())
			throw "Floor empty\n";
		/*
		if(tableHand.isEmpty())
			throw "Table empty\n";
		*/
		vector<pair<Card, vector<vector<Card> > > > combinations;
		vector<Card> V;

		for(int i=0; i<tableHand.size(); i++){
			V.push_back(tableHand[i]);
		}
		vector<HouseCard>::const_iterator it = tableHouses.begin();
		while(it != tableHouses.end()){
			if(!it->isPermanent && (it->created_by != getCurrentPlayerIndex())){ //Player can't change his own house
				V.push_back(Card(it->value, '\0'));
			}
			it++;
		}
		findCombinations(V, combinations);
		return combinations;
	}

	vector<pair<Card, vector<vector<Card> > > > createCombinations(Card C) const{ //including C but not adding it to the floor
		if(isFloorEmpty())
			throw "Floor empty\n";
		/*
		if(tableHand.isEmpty())
			throw "Table empty\n";
		*/
		vector<pair<Card, vector<vector<Card> > > > combinations;
		vector<Card> V;
		V.push_back(C);
		for(int i=0; i<tableHand.size(); i++){
			V.push_back(tableHand[i]);
		}
		vector<HouseCard>::const_iterator it = tableHouses.begin();
		while(it != tableHouses.end()){
			if(!it->isPermanent && (it->created_by != getCurrentPlayerIndex())){ //Player can't change his own house
				V.push_back(Card(it->value, '\0'));
			}
			it++;
		}
		findCombinations(V, combinations);
		return combinations;
	}

	vector<vector<Card> > createCombinationsOnlyFor(Card C) const{ //If card is found on the floor, then combinations for the one on floor. Otherwise, normally
		if(isFloorEmpty())
			throw "Floor empty\n";
		/*
		if(tableHand.isEmpty())
			throw "Table empty\n";
		*/
		vector<Card> V;
		int index = tableHand.findCard(C);
		if(index == -1){ // Card not on the floor
			for(int i=0; i<tableHand.size(); i++)
				V.push_back(tableHand[i]);
		}
		else{
			for(int i=0; i<tableHand.size(); i++){
				if(i == index)
					continue;
				V.push_back(tableHand[i]);
			}
		}
		vector<HouseCard>::const_iterator it = tableHouses.begin();
		while(it != tableHouses.end()){
			if(!it->isPermanent && (it->created_by != getCurrentPlayerIndex())){ //Player can't change his own house
				V.push_back(Card(it->value, '\0'));
			}
			it++;
		}
		sort(V.begin(), V.end());
		vector<vector<Card> > combn;
		findCombinationsHelper(V,0,V.size(),vector<Card>(),C.getValue(),combn);
		return combn;
	}

	vector<pair<int, vector<vector<Card> > > > createHouseCombinations(Card C, vector<int> houseValues) const{
		//houseValues is the vector containing the values to be used as target_sum
		//if houseValues is empty, this function fills it with house values.. 9,10,11,12,13
		if(isFloorEmpty())
			throw "Floor empty\n";
		/*
		if(tableHand.isEmpty())
			throw "Table empty\n";
		*/

		if(houseValues.empty()){
			for(int i=9;i<14;i++)
				houseValues.push_back(i);
		}

		vector<Card> V;
		int index = tableHand.findCard(C);
		if(index == -1){ // Card not on the floor
			for(int i=0; i<tableHand.size(); i++)
				V.push_back(tableHand[i]);
		}
		else{
			for(int i=0; i<tableHand.size(); i++)
				V.push_back(tableHand[i]);
		}
		vector<HouseCard>::const_iterator it = tableHouses.begin();
		while(it != tableHouses.end()){
			if(!it->isPermanent && (it->created_by != getCurrentPlayerIndex())){ //Player can't change his own house
				V.push_back(Card(it->value, '\0'));
			}
			it++;
		}
		sort(V.begin(), V.end());
		vector<pair<int, vector<vector<Card> > > > combn; //value, combinations

		vector<Card> vec;
		vec.push_back(C); //Because we definitely want the Card C to be in the combination

		for(int i=0;i<houseValues.size();i++){
			vector<vector<Card> > temp;
			findCombinationsHelper(V,0,V.size(),vec,houseValues[i],temp);
			combn.push_back(make_pair(houseValues[i], temp));
		}
		return combn;
	}
	
	int findHouse(int val) const{
		val = houseValueValidator(val);
		if(val == -1)
			return val;
		for(int i=0;i<tableHouses.size();i++){
			if(tableHouses[i].value == val)
				return i;
		}
		return -1;
	}

	/*
	bool incrementHouse(int val, const Card &C) {
		int index = findHouse(val);
		if(index == -1 || tableHouses[index].isPermanent || (tableHouses[index].created_by==getCurrentPlayerIndex()))
			return false;
		int new_val = houseValueValidator(val+C.getValue());
		if(new_val == -1)
			return false;
		HouseCard &HC = tableHouses[index];
		HC.value = new_val;
		HC.combinations.back().push_back(C); //back because house is temporary.. only 1 combination
		HC.isPermanent = true;
		HC.created_by = getCurrentPlayerIndex();
		return true;
	}*/

	bool cementHouse(int val, const vector<Card> &V){
		int index = findHouse(val);
		if(index == -1 || V.empty())
			return false;
		int valid = combinationValueValidator(val,V);
		if(valid == -1)
			return false;
		HouseCard &HC = tableHouses[index];
		HC.combinations.push_back(V);
		HC.isPermanent = true;
		return true;
	}

	bool createHouse(int val, const vector<Card> &V) {
		val = houseValueValidator(val);
		if(val == -1 || V.empty())
			return false;
		int index = findHouse(val);
		if(index != -1){
			return false;
			//return cementHouse(val,V); //adding combination to combinations vector of the house
		}
		val = combinationValueValidator(val,V);
		if(val == -1)
			return false;
		/*
		Update: Not deleting cards because it is easier to remove from the game as some cards are to be deleted from pHand while other from table
		//Deleting the cards participating in the combination(V) from the tableHand
		try{
			removeCardsFromTable(V);
		}
		catch(const char *msg){
			cout << msg;
			throw;
		}*/

		//Finding the combinations of loose cards that add up to the house value; Cementing the house
		//Modify this for user discretion.. Can also cement the house with combination which has most points!
	//	vector<vector<Card> > combn = createCombinationsOnlyFor(Card(val,'\0'));
		/*
		cout << "******************************\n";
		for(int a=0;a<combn.size();a++){
			for(int b=0;b<combn[a].size();b++){
				cout << combn[a][b].getValue() << combn[a][b].getSuite() << " ";
			}
			cout << endl;
		}
		cout << "******************************\n\n";
		*/
		//for now cementing with random combination
		
		/*
			Note: Need to cement the house with all the combinations having non-overlapping cards in a way that maximum points are alloted to the house.
			FAK
		
		 *****
		*/
		
		//Adding to the vector later because in createCombinationsOnlyFor() we're taking the house cards as well, which would include the house card we're creating combinations for (temporary)
		tableHouses.push_back(HouseCard(V,getCurrentPlayerIndex()));
		HouseCard &HC = tableHouses.back();
		HC.created_by = getCurrentPlayerIndex();
		
		//*****
	//	if(combn.size() > 0)
	//		HC.addCombination(combn[0]); //will validate and change isPermanent to true
	//	
		//Note: Commented the cementing with random combn part because we're handling the deletion of cards from table in the caller's class(sweep.h) as it will be easier there to identify pHandcards, tableCards ,temphousecards
		return true;
	}

	vector<vector<Card> > removeHouse(int val) {
		int index = findHouse(val);
		if(index == -1){
			//throw "House requested to be dissolved not found.\n";
			return vector<vector<Card> >();
		}
		
		vector<HouseCard>::iterator it = tableHouses.begin();
		it += index;
		vector<vector<Card> > temp = it->combinations;
		tableHouses.erase(it);
		return temp;
	}

	bool isFloorEmpty() const{
		return (isHandEmpty() && ((tableHouses.size()==0) ? true : false));
	}

	friend class Sweep;
	friend ostream & operator<<(ostream &, const SweepTable &);
};

ostream & operator<<(ostream &out, const SweepTable &T){
	out << "-------- TABLE --------" << endl;
	out << "Number Of Players: " << T.numberOfPlayers() << " [";
	for(int i=0;i<T.players.size();i++){
		out << T.players[i].getName();
		if(i+1 != T.players.size())
			out << ", ";
	}
	cout << "]" << endl;
	out << "Deck Size: " << T.tableDeckSize() << endl;
	out << "Table Hand Cards: " << endl;
	out << T.tableHand << endl;
	out << "Table Houses: " << endl;
	for(int i=0; i<T.tableHouses.size(); i++){
		out << T.tableHouses[i];
	}
	return out;
}

#endif
