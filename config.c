#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "players.h"
#include "ai.h"
#define MAX_LINE 35

const char AI[] = "EA";
const char NOT_AI[] = "HU";

void ReadGameSettingsPlayer(FILE *config_file, Player *player) {
    char line[MAX_LINE] = { 0 };
    char *aux = NULL;
    int i = 0;
    
    if (fgets(line, MAX_LINE, config_file) == NULL) {
        printf("Error reading configuration file!\n");
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
                    printf("Error reading configuration file!\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 1:
                if (aux == NULL || strlen(aux) > MAX_NAME) {
                    printf("Error reading configuration file!\n");
                    exit(EXIT_FAILURE);
                }
                
                strcpy(player->name, aux);
                break;
            case 2:
                if (sscanf(aux, "%f", &(player->money)) != 1) {
                    printf("Error reading configuration file!\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 3:
                if (sscanf(aux, "%f", &(player->bet)) != 1) {
                    printf("Error reading configuration file!\n");
                    exit(EXIT_FAILURE);
                }
                break;
        }
        
        aux = strtok(NULL, "-");
        i += 1;
    } while (aux != NULL);
    
    player->cards = NULL;
    player->score = 0;
    player->hand_size = 0;
    player->games_result.won = 0;
    player->games_result.tied = 0;
    player->games_result.lost = 0;
    player->surrender = false;
}

void PromptNewPlayer(Player *newPlayer) {
    short notValid = 1; // used for parameter value check
    char buffer[MAX_LINE] = { 0 }; // buffer for fgets
    char aux = 0;
    
    /* While the parameter entered is not valid prompt for it */
    while (notValid) {
        /* Prompts the parameter */
        printf("Player Name: ");
        
        notValid = fgets(buffer, MAX_LINE, stdin) == NULL || strlen(buffer) > MAX_NAME;
    }
    
    buffer[strlen(buffer) - 1] = '\0';
    
    strcpy(newPlayer->name, buffer);
    
    notValid = 1; // resets not valid
    
    /* While the parameter entered is not valid prompt for it */
    while (notValid) {
        /* Prompts the parameter */
        printf("AI? (Y/n) ");
        fgets(buffer, MAX_LINE, stdin);
        sscanf(buffer, "%c", &aux);
        
        notValid = aux != 'Y' && aux != 'n';
        
        /* If not valid show error */
        if (notValid) {
            printf("\n** Not valid (Y/n). **\n\n");
        }
    }
    
    newPlayer->ai = aux == 'Y';
    
    notValid = 1; // resets not valid
    
    /* While the parameter entered is not valid prompt for it */
    while (notValid) {
        /* Prompts the parameter */
        printf("Initial money: ");
        fgets(buffer, MAX_LINE, stdin);
        
        notValid = sscanf(buffer, "%f", &(newPlayer->money)) != 1 || newPlayer->money < 0;
        
        /* If not valid show error */
        if (notValid) {
            printf("\n** Sorry, invalid initial money. **\n\n");
        }
    }
    
    notValid = 1;
    
    while (notValid) {
        /* Prompts the parameter */
        printf("Bet amount: ");
        fgets(buffer, MAX_LINE, stdin);
        
        notValid = sscanf(buffer, "%f", &(newPlayer->bet)) != 1 || newPlayer->bet > newPlayer->money || newPlayer->bet < 0;
        
        /* If not valid show error */
        if (notValid) {
            printf("\n** Sorry, invalid bet amount. **\n\n");
        }
    }
}

void AddNewPlayer(Player_node **players, int position) {
    Player_node *aux = *players;
    Player *newPlayer = NULL;
    
    while (aux != NULL) {
        if (aux->player.position == position) {
            return;
        }
        aux = aux->next;
    }
    
    newPlayer = (Player *) malloc(sizeof(Player));
    
    if (newPlayer == NULL) {
        ERROR_MESSAGE();
        exit(EXIT_FAILURE);
    }
    
    
    PromptNewPlayer(newPlayer);
    
    newPlayer->cards = NULL;
    newPlayer->score = 0;
    newPlayer->hand_size = 0;
    newPlayer->games_result.won = 0;
    newPlayer->games_result.tied = 0;
    newPlayer->games_result.lost = 0;
    newPlayer->surrender = false;
    newPlayer->position = position;
    
    insert_sorted_player_node(players, *newPlayer);
    
    free(newPlayer);
    
}

void ReadAIActions(FILE *ai_strategy, AIAction ***ai_actions) {
    int currentColumn = 0, currentRow = 0;
    char c;
    
    *ai_actions = (AIAction **) malloc(sizeof(AIAction *) * AIACTIONS_ROWS);
    
    if (*ai_actions == NULL) {
        ERROR_MESSAGE();
        
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < AIACTIONS_ROWS; i++) {
        (*ai_actions)[i] = (AIAction *) malloc(sizeof(AIAction) * AIACTIONS_COLUMNS);
        
        if ((*ai_actions)[i] == NULL) {
            ERROR_MESSAGE();
            
            exit(EXIT_FAILURE);
        }
    }
    
    while ((c = fgetc(ai_strategy)) != EOF) {
        if (c == '\n' || c == '\r') {
            currentRow += 1;
            currentColumn = 0;
            continue;
        }
        
        if (c > '4' || c < '0') {
            printf("Error opening configuration file!\n");
            
            exit(EXIT_FAILURE);
        }
        
        if (currentRow > AIACTIONS_ROWS || currentColumn > AIACTIONS_COLUMNS) {
            break;
        }
        
        (*ai_actions)[currentRow][currentColumn] = (AIAction) (c - '0');
        
        currentColumn += 1;
    }
}

void GameSettings(char *config_file, char *ai, int *decks, Player_node **resultPlayers, AIAction ***ai_actions) {
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
    
    ReadAIActions(ai_strategy, ai_actions);
    
    if (fgets(line, MAX_LINE, game_file) == NULL) {
        printf("Error reading configuration file!\n");
        exit(EXIT_FAILURE);
    }
    
    tmp = strtok(line, "-");
    
    if (sscanf(tmp, "%d", decks) == 0) {
        printf("Error reading configuration file!\n");
        exit(EXIT_FAILURE);
    }
    
    tmp = strtok(NULL, "-");
    
    if (sscanf(tmp, "%d", &players) == 0) {
        printf("Error reading configuration file!\n");
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
        
        ReadGameSettingsPlayer(game_file, newPlayer);
        newPlayer->position = i;
        
        insert_player_node(&head, &aux, *newPlayer);
        
        free(newPlayer);
    }
    
    *resultPlayers = head;
    
    fclose(game_file);
    fclose(ai_strategy);
    
}