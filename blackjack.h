#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "players.h"
#include "cards.h"

#define BLACKJACK_INITIAL_CARDS 2  // initially dealt cards
#define BLACKJACK_MAX_SCORE 21  // max score
#define Bust(player) player.score > BLACKJACK_MAX_SCORE  // bust check macro
#define Blackjack(player) (player.score == BLACKJACK_MAX_SCORE && player.hand_size == BLACKJACK_INITIAL_CARDS)  // blackjack check macro
#define STATISTICS_FILE_NAME "stats.txt" // Stats file name

void GetBankroll_GameResults(Player* house, Player_node **head);
void FinishTurn(Card_node **deck_head, int numberOfDecks, Player* house, Player_node **head, int *hilo, int *cardsDealt);
void NewTurn(Card_node **deck_head, int numberOfDecks, Player_node **players, Player_node **removedPlayers, Player_node **currentPlayerNode, Player *house, int *hilo, int *cardsDealt);
void PlayHouse(Player *house, Card_node **deck_head, int numberOfDecks, int *hilo, int *cardsDealt);
Player_node* Hit(Card_node** deck_head, Player_node* current_player, int numberOfDecks, int *hilo, int *cardsDealt);
Player_node* Stand(Player_node* current_player);
Player_node* Double(Card_node **deckhead, int decks, Player *house, Player_node *current_player, int *hilo, int *cardsDealt);
Player_node* Surrender(Player_node *current_player);
void Bet(Player_node *head);
void DealCards(Card_node** deck_head, Player_node* head, Player* house, int numberOfDecks, int numberOfCardsToDeal, int *hilo, int *cardsDealt);
void WriteMoneyAndStatsToFile(Player_node *players, Player *house, bool turn_ended);

#endif
