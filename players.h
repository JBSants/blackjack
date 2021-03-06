#ifndef PLAYERS_H
#define PLAYERS_H

#include <stdbool.h>
#include "cards.h"

#define MIN_BET_VALUE 2
#define MAX_BET_PERCENTAGE 0.25
#define MIN_MONEY 10
#define MAX_MONEY 500
#define MAX_NAME 9
#define MIN_PLAYERS 1
#define MAX_PLAYERS 4

typedef struct {
    int won;
    int lost;
    int tied;
} Results;

typedef struct {
    char name[MAX_NAME];
    float money;
    float bet;
    float initialBet;
    Card_node *cards;
    int hand_size;
    int position;
    Results games_result;
    bool ai; // computer or artificial inteligence
    bool surrender;
    short score;
} Player;

typedef struct node_player {
    Player player;
    struct node_player *next;
} Player_node;

void GetScore(Player* current_player);
void GetPlayerListScore(Player_node *head);
void add_player(Player_node** head, Player data);
Player_node *create_player_node();
void insert_player_node(Player_node **head, Player_node **tail, Player data);
void insert_sorted_player_node(Player_node **head, Player data);
void erase_player_list(Player_node* head);
void join_player_node(Player_node** head, Player_node* jointo, int position);
Player_node *take_player_node(Player_node** head, int position);

#endif
