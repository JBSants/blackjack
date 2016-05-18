#ifndef DECK_H
#define DECK_H

#include "cards.h"

#define DECK_MAX_CARDS 52

Card_node* DeckMaker(int decks);
void ShuffleCards(Card_node** deck_head, int decks);
void EraseDeck(Card_node* deck_head);
int GetHiloScore(Card card);
Card_node *NextCard(Card_node **deck_head, int numberOfDecks, int *hilo, int *cardsDealt);

#endif
