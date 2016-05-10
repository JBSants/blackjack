#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "players.h"
#include "cards.h"

void GetBankroll_GameResults(Player* house, Node_player* head, int bet);
Node_player* Hit(Card_node** deck_head, Player* house, int decks, Node_player* current_player, int option);
Node_player* Stand(Player* house, int decks, Node_player* current_player, int option);
Node_player* Stand(Player* house, int decks, Node_player* current_player, int option);
Node_player* Double(Card_node **deckhead, int decks, Player *house, Node_player *current_player);
Node_player* Surrender(Node_player *current_player);
void Bet(Node_player *head);

#endif
