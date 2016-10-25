#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include <string>

class Player{
	string name;
	Hand currentHand;
	int score;
	bool hasWon;

	static string defaultName(){
		int count = playerCount;
		string str = "BOT ";
		while(count>9){
			str += ('0' + count/10);
			count /= 10;
		}
		str += '0' + count;
		return str;
	}

	public:
	
	static int playerCount;
	
	Player() : score(0) , hasWon(false) {
		name = defaultName();
		playerCount++;
	}
	Player(string n) : name(n) , score(0) , hasWon(false){
		playerCount++;
	}

	string getName() const{
		return name;
	}

	int getScore() const{
		return score;
	}

	void updateScore(int n){
		score += n;
	}

	void sortHandNormally(){
		currentHand.sortNormally();
	}

	void sortHandSuites(){
		currentHand.sortSuites();
	}

	void sortHandValues(){
		currentHand.sortValues();
	}

	void giveCards(const Card &C){
		currentHand.addCards(C);
	}

	void giveCards(const vector<Card> &V){
		currentHand.addCards(V);
	}

	vector<Card> playKCards(int k){
		return currentHand.getKCards(k);
	}

	Card playCards(const Card &C){
		return currentHand.removeCards(C);
	}

	vector<Card> playCards(const vector<Card> &V){
		return currentHand.removeCards(V);
	}

	vector<int> hasCardsOfValueAll(int k) const{
		if(currentHand.isEmpty() || (k<=0 || k>13))
			return vector<int>();
		return currentHand.findAllOfValue(k);
	}

	const vector<Card> & getPlayerHand() const{
		return currentHand.getHand();
	}

	friend ostream & operator<<(ostream &,const Player &);
};

int Player::playerCount = 0;

ostream & operator<<(ostream &out, const Player &P){
	cout << endl << endl;
	cout << "----- " << P.name << " -----" << endl;
	cout << "Score: " << P.score << endl;
	cout << "Has Won: " << P.hasWon << endl;
	cout << "Hand: " << endl << P.currentHand << endl;
	return out;
}

#endif
