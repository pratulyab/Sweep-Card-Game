#include <stdlib.h>
#include "sweep.h"

int main(){
	system("clear");
	cout << "\t\t\t\t==================================== SEEP CARD GAME ======================================\n";
	Sweep S(2,1);
	int housecall = S.start();
	while(1){
		cout << "1) PICK\n2) COMBINE\n3) THROW\n4) COVER\n";
		int inp;
		cin >> inp;
		switch(inp){
			case 1:
				if(!S.pick(housecall))
					continue;
				break;
			case 2:
				if(!S.combine(housecall))
					continue;
				break;
			case 3:
				if(!S.thro(housecall))
					continue;
				break;
			case 4:
				if(!S.cover(housecall))
					continue;
				break;
			case -1:
				continue;
			default:
				cout << "Wrong input, Try again.\n";
		}
		housecall = 0;
		S.sweeped();
		S.proceed();
		system("clear");
		cout << "\t\t\t\t==================================== SEEP CARD GAME ======================================\n";	
		S.displayCurrentPlayer();
		S.displayTable();
	}
return 0;
}
