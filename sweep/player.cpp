#include "deck.h"
#include "player.h"

int main(){
	Player P;
	cout << P;
	Deck D;
	D.shuffle();
	P.giveCards(D.drawKCards(6));
	cout << P;
	P.giveCards(D.drawKCards(6));
	P.sortHandNormally();
	cout << P;
	P.giveCards(D.drawKCards(6));
	P.sortHandSuites();
	cout << P;
	P.sortHandValues();
	cout << P;
	return 0;
}
