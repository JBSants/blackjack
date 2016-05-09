#ifndef PLAYERS_H
#define PLAYERS_H

#include <stdbool.h>
#include <cards.h>

#define MAX_NOME 9

typedef struct {
    int won;
    int lost;
    int tied;
} Results;

typedef struct {
    char nome[MAX_NOME];
    float money;
    float bet;
    CardStack *cards;
    Results games_result;
    bool ai; // computer or artificial inteligence
    short score;
} Player;

typedef struct node_player {
    Player player;
    struct node_player *next;
} Node_player;


#endif
