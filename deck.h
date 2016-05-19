#ifndef DECK_H
#define DECK_H

#include "cards.h"

#define DECK_MAX_CARDS 52
#define MAX_DECKS 8
#define MIN_DECKS 4
#define MAX_SUIT_CARDS 13
#define MAX_SUITS 4

Card_node* DeckMaker(int decks);
void ShuffleCards(Card_node** deck_head, int decks);
void EraseDeck(Card_node* deck_head);
int GetHiloScore(Card card);
Card_node *NextCard(Card_node **deck_head, int numberOfDecks, int *hilo, int *cardsDealt);

#endif
