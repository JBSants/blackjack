#ifndef DECK_H
#define DECK_H

#include "cards.h"

Card_node* CreateDeck(int decks);
void ShuffleCards(Card_node** deck_head, int decks, int times);

#endif
