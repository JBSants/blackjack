all:
	gcc *.c -g -I/Users/jbsants/SDL/include -Wall -pedantic -std=c99 -L/Users/jbsants/SDL/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -o blackjack