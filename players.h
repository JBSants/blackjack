#ifndef PLAYERS_H
#define PLAYERS_H

#include <stdbool.h>
#include "cards.h"

#define MAX_NAME 9

typedef struct {
    int won;
    int lost;
    int tied;
} Results;

typedef struct {
    char name[MAX_NAME];
    float money;
    float bet;
    Card_node *cards;
    int hand_size;
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
Stat add_player(Player_node** head, Player data);
Player_node *create_player_node();
Stat insert_player(Player_node **head, Player_node **tail, Player data);
void erase_player_list(Player_node* head);

#endif
