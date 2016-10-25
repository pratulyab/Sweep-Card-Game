#ifndef TABLE_H
#define TABLE_H

#include "player.h"

class Table{
	
	protected:
	vector<Player> players;
	int currentPlayer;
	int dealerIndex;
	Deck tableDeck;
	Hand tableHand;

	public:
	Table() : currentPlayer(-1) , dealerIndex(-1) {tableDeck.shuffle();}
	Table(int nop, int dealer) : currentPlayer(-1) { //Pass dealer as normal indexing and not as zero indexing
		
		if(nop < 0)
			throw "Invalid number of players to start the game.";
		if(dealer<=0 || dealer>nop)
			throw "Invalid dealer index provided.";
		
		dealerIndex = dealer-1;
		
		for(int i=0 ; i<nop ; i++){
			players.push_back(Player());
		}
		tableDeck.shuffle();
	}
	Table(int nop, int dealer, const vector<string> &names) : currentPlayer(-1) {
		
		if(nop < 0)
			throw "Invalid number of players to start the game.";
		if(dealer<=0 || dealer>nop)
			throw "Invalid dealer index provided.";
		
		dealerIndex = dealer-1;

		int i=0;
		for(int j=0 ; i<nop && j<names.size() ; i++,j++){
			players.push_back(Player(names[j]));
		}
		while(i<nop){
			players.push_back(Player());
			i++;
		}
		tableDeck.shuffle();
	}

	void addCardsToTable(const Card &C){
		tableHand.addCards(C);
	}

	void addCardsToTable(const vector<Card> &V){
		tableHand.addCards(V);
	}

	Card removeCardsFromTable(const Card &C){
		try{
			return tableHand.removeCards(C);
		}
		catch(const char * msg){
			throw;
		}
	}

	vector<Card> removeCardsFromTable(const vector<Card> &V){
		try{
			return tableHand.removeCards(V);
		}
		catch(const char * msg){
			throw;
		}
	}

	void distributeKCardsEachFromDeck(int k){
		
		if(k<0 || (tableDeckSize() - (numberOfPlayers()*k))<0 )
			throw "Invalid number of cards requested to distribute to each player on table.";
		if(k==0)
			return;

		//give first to the current player
		int n = numberOfPlayers();
		int i = currentPlayer;
		while(n--){
			getKthPlayer(i+1).giveCards(tableDeck.drawKCards(k));
			i++;
			if(i == numberOfPlayers())
				i = 0;
		}
	}

	int tableDeckSize() const{ // Number of cards left in deck
		return tableDeck.size();
	}

	int numberOfPlayers() const{
		return players.size();
	}

	Player & getKthPlayer(int k){ //Provide normal k (not zero indexing based)
		if(k<=0 || k>numberOfPlayers())
			throw "Out of bound error while fetching kth player";
		return players[k-1];
	}

	const Player & getKthPlayer(int k) const{ //Provide normal k (not zero indexing based)
		if(k<=0 || k>numberOfPlayers())
			throw "Out of bound error while fetching kth player";
		return players[k-1];
	}

	Player & getCurrentPlayer(){
		if(currentPlayer == -1)
			throw "Invalid current player.";
		return players[currentPlayer];
	}

	const Player & getCurrentPlayer() const{
		if(currentPlayer == -1)
			throw "Invalid current player.";
		return players[currentPlayer];
	}

	int getCurrentPlayerIndex() const{ // Returns Zero Indexing
		if(!numberOfPlayers()){
			throw "No players";
		}
		return currentPlayer;
	}

	Player & changeCurrentPlayer(int k){
		if(k<=0 || k>numberOfPlayers())
			throw "Invalid index to change current player to.";

		currentPlayer = k-1;
		return players[currentPlayer];
	}

	int nextPlayerIndex() const{ // Returns Zero Indexing + 1
		if(currentPlayer == -1)
			return -1;
		return ((currentPlayer+1 == numberOfPlayers()) ? 0 : currentPlayer+1)+1;
	}

	void printPlayer(int k) const{ // Provide normal k
		if(k<=0 || k>numberOfPlayers())
			throw "Invalid k provided to print player";
		cout << players[k-1] << endl;
	}

	int getDealerIndex() const{
		return dealerIndex+1;
	}

	int findCard(const Card &C) const{
		return tableHand.findCard(C);
	}

	bool isHandEmpty() const{
		return ( (tableHand.size() == 0) ? true : false);
	}

	vector<Card> temp(int k) { // redundant fxn defined for use in sweeptable.cpp
		return tableDeck.drawKCards(k);
	}
	
	friend ostream & operator<<(ostream &,const Table &);
};

ostream & operator<<(ostream &out, const Table &T){
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
	return out;
}

#endif
