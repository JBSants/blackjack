#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "players.h"
#define MAX_LINE 35

const char AI[] = "EA";
const char NOT_AI[] = "HU";

void ReadGameSettingsLine(FILE *config_file, Player *player) {
    char line[MAX_LINE] = { 0 };
    char *aux = NULL;
    int i = 0;
    
    if (fgets(line, MAX_LINE, config_file) == NULL) {
        printf("Error reading configuration file!");
        exit(EXIT_FAILURE);
    }
    
    aux = strtok(line, "-");
    
    if (aux == NULL) {
        ERROR_MESSAGE();
        exit(EXIT_FAILURE);
    }
    
    do {
        switch (i) {
            case 0:
                if (strcmp(aux, NOT_AI) == 0) {
                    player->ai = false;
                } else if (strcmp(aux, AI) == 0){
                    player->ai = true;
                } else {
                    printf("Error reading configuration file!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 1:
                if (aux == NULL || strlen(aux) > MAX_NAME) {
                    printf("Error reading configuration file!");
                    exit(EXIT_FAILURE);
                }
                
                strcpy(player->name, aux);
                break;
            case 2:
                if (sscanf(aux, "%f", &(player->money)) != 1) {
                    printf("Error reading configuration file!");
                    exit(EXIT_FAILURE);
                }
                break;
            case 3:
                if (sscanf(aux, "%f", &(player->bet)) != 1) {
                    printf("Error reading configuration file!");
                    exit(EXIT_FAILURE);
                }
                break;
        }
        
        player->cards = NULL;
        player->score = 0;
        player->hand_size = 0;
        player->games_result.won = 0;
        player->games_result.tied = 0;
        player->games_result.lost = 0;
        player->surrender = false;
        
        aux = strtok(NULL, "-");
        i += 1;
    } while (aux != NULL);
}

void GameSettings(char *config_file, char *ai, int *decks, Player_node **resultPlayers) {
    FILE* game_file, *ai_strategy;
    Player_node *aux;
    Player_node *head = NULL;
    int players;
    char line[MAX_LINE] = { 0 };
    char *tmp = NULL;
    
    game_file = fopen(config_file,"r");
    
    if(game_file==NULL){
        printf("Error opening configuration file!\n");
        exit(EXIT_FAILURE);
    }
    
    ai_strategy = fopen(ai, "r");
    
    if(ai_strategy == NULL){
        printf("Error opening AI strategy file!\n");
        exit(EXIT_FAILURE);
    }
    
    if (fgets(line, MAX_LINE, game_file) == NULL) {
        printf("Error reading configuration file!");
        exit(EXIT_FAILURE);
    }
    
    tmp = strtok(line, "-");
    
    if (sscanf(tmp, "%d", decks) == 0) {
        printf("Error reading configuration file!");
        exit(EXIT_FAILURE);
    }
    
    tmp = strtok(NULL, "-");
    
    if (sscanf(tmp, "%d", &players) == 0) {
        printf("Error reading configuration file!");
        exit(EXIT_FAILURE);
    }
    
    if(*decks < 4 || *decks > 8 || players < 1 || players > 4){
        printf("Not valid players or deck parameters!\n");
        exit(EXIT_FAILURE);
    }
    
    
    aux = head;
    
    for (int i = 0; i < players; i++) {
        Player *newPlayer = (Player *) malloc(sizeof(Player));
        
        if (newPlayer == NULL) {
            ERROR_MESSAGE();
            exit(EXIT_FAILURE);
        }
        
        ReadGameSettingsLine(game_file, newPlayer);
        
        insert_player(&head, &aux, *newPlayer);
        
        free(newPlayer);
    }
    
    *resultPlayers = head;
    
}
