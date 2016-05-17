#ifndef DECK_H
#define DECK_H

#include "cards.h"

Card_node* DeckMaker(int decks);
void ShuffleCards(Card_node** deck_head, int decks);
void EraseDeck(Card_node* deck_head);

#endif
