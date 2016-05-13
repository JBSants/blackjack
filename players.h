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
    CardStack_node *cards;
    int hand_size;
    Results games_result;
    bool ai; // computer or artificial inteligence
    bool surrender;
    short score;
} Player;

typedef struct node_player {
    Player player;
    struct node_player *next;
} Node_player;

void GetScore(Node_player* current_player);

#endif
