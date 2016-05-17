#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "players.h"
#include "cards.h"

#define BLACKJACK_INITIAL_CARDS 2
#define MIN_BET_VALUE 2
#define Bust(player) player.score > 21

void GetBankroll_GameResults(Player* house, Player_node **head);
void FinishTurn(Card_node **deck_head, int numberOfDecks, Player* house, Player_node **head);
void NewTurn(Card_node **deck_head, int numberOfDecks, Player_node **players, Player_node **removedPlayers, Player_node **currentPlayerNode, Player *house);
void PlayHouse(Player *house, Card_node **deck_head, int numberOfDecks);
Player_node* Hit(Card_node** deck_head, Player_node* current_player, int numberOfDecks);
Player_node* Stand(Player_node* current_player);
Player_node* Double(Card_node **deckhead, int decks, Player *house, Player_node *current_player);
Player_node* Surrender(Player_node *current_player);
void Bet(Player_node *head);
void DealCards(Card_node** deck_head, Player_node* head, Player* house, int numberOfDecks, int numberOfCardsToDeal);
Card_node *NextCard(Card_node **deck_head, int numberOfDecks);
void WriteMoneyAndStatsToFile(Player_node *players, Player *house);

#endif
