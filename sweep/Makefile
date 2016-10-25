sweep: sweep.o
	g++ sweep.o -o play

sweep.o: sweep.cpp sweep.h sweeptable.h table.h housecard.h player.h hand.h deck.h card.h
	g++ -c sweep.cpp

all: sweep

clean:
	rm play *.o
