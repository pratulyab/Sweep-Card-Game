#include "deck.h"

int main(){
	Deck D;
	D.shuffle();
	cout << D;
	cout << "---------------------------------\n";
	D.sort();

	cout << D;

	return 0;
}
