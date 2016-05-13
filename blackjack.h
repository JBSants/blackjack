#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "players.h"
#include "cards.h"

#define BLACKJACK_INITIAL_CARDS 2
#define Bust(player) player.score > 21

void GetBankroll_GameResults(Player* house, Player_node* head, int bet);
Player_node* Hit(Card_node** deck_head, Player_node* current_player);
Player_node* Stand(Player* house, Player_node* current_player);
Player_node* Double(Card_node **deckhead, int decks, Player *house, Player_node *current_player);
Player_node* Surrender(Player_node *current_player);
void Bet(Player_node *head);
int DealCards(Card_node** deck_head, Player_node* head, Player* house, int decks, int numberOfCardsToDeal, DealOption option);
Card_node *NextCard(Card_node **deck_head);

#endif
