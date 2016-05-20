#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "players.h"
#include "deck.h"

#define MAX_LINE 35
#define CONFIG_SEPARATOR "-"

void ConfigurationFileError() {
    printf("Error reading configuration file!\n");
    
    exit(EXIT_FAILURE);
}


/* FUNCTION NAME:ReadGameSettingsPlayer
*  DESCRIPTION: Reads players parameters from the configuration file */
void ReadGameSettingsPlayer(FILE *config_file, Player *player) {
    char line[MAX_LINE] = { 0 };
    char *aux = NULL;
    int i = 0;
    
   /* Reads a line from the configuration file, and if 
   not possible exits the main program */
    if (fgets(line, MAX_LINE, config_file) == NULL) {
        ConfigurationFileError();
    }
    
    /* Breaks the line into the substrings (tokens) separated
    by '-' and exits the main program if a first token isn't found */
    aux = strtok(line, CONFIG_SEPARATOR);
    if (aux == NULL) {
        ALLOCATION_ERROR_MESSAGE();
        
        exit(EXIT_FAILURE);
    }
    
    do {
        switch (i) { // Switches through all the different player parameters
            case 0: // First Parameter: player's type
                /* Check if player is human */
                if (strcmp(aux, NOT_AI) == 0) {
                    player->ai = false; // turns ai off (is human)
                /* Check if player is artificial intelligence */    
                } else if (strcmp(aux, AI) == 0){
                    player->ai = true; // turns ai on
                /* If it's not an expected type exits the main program */ 
                } else {
                    ConfigurationFileError();
                }
                break;
            case 1: // Second Parameter: player's name
                /* Checks if the name was found in the file
                and if the name as an expected size, if not
                exits the main program */ 
                if (aux == NULL || strlen(aux) > MAX_NAME) {
                    ConfigurationFileError();
                }
                
                strcpy(player->name, aux); // saves the player's name
                break;
            case 2: // Third Parameter: player's initial money
                /* Checks if the parameter is of type float, if
                not exits the main program, if it is saves it */
                if (sscanf(aux, "%f", &(player->money)) != 1) {
                    ConfigurationFileError();
                }
                /* Sets boundaries to the money value, and if 
                they're not respected the main program is aborted */
                if (player->money < MIN_MONEY || player->money > MAX_MONEY) {
                    ConfigurationFileError();
                }
                
                break;
            case 3: // Fourth Parameter: player's initial bet
                /* Checks if the parameter is of type float, if
                not exits the main program, if it is saves it */
                if (sscanf(aux, "%f", &(player->bet)) != 1) {
                    ConfigurationFileError();
                }
                
                /* Sets boundaries to the bet value, and if they're 
                not respected the main program is aborted */
                if (player->bet < MIN_BET_VALUE || player->bet > (MAX_BET_PERCENTAGE * player->money)) {
                    ConfigurationFileError();
                }
                
                player->initialBet = player->bet; // Saves the player's bet as initial bet
                break;
        }
        
        aux = strtok(NULL, CONFIG_SEPARATOR); // Searches for the next token
        i += 1;
    } while (aux != NULL); // while there's tokens
    
    /* Initializes remaining player's fields */
    player->cards = NULL;
    player->score = 0;
    player->hand_size = 0;
    player->games_result.won = 0;
    player->games_result.tied = 0;
    player->games_result.lost = 0;
    player->surrender = false;
}

void PromptNewPlayer(Player *newPlayer, Player_node *players) {
    short notValid = 1; // used for parameter value check
    char buffer[MAX_LINE] = { 0 }; // buffer for fgets
    char aux = 0;
    
    /* While the parameter entered is not valid prompt for it */
    while (notValid) {
        /* Prompts the parameter */
        printf("Player Name: ");
        
        notValid = fgets(buffer, MAX_LINE, stdin) == NULL || strlen(buffer) > MAX_NAME;
        
        while (players != NULL) {
            if (strcmp(players->player.name, buffer)) {
                notValid = 1;
            }
            
            players = players->next;
        }
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

/* FUNCTION NAME:AddNewPlayer
*  DESCRIPTION: Adds new player with information provided by 
*  the user to list in a specified position */
void AddNewPlayer(Player_node **players, int position) {
    Player_node *aux = *players;
    Player *newPlayer = NULL;
    
    /* If the position specified is already been taken return */
    while (aux != NULL) {
        if (aux->player.position == position) {
            return;
        }
        aux = aux->next;
    }
    
    /* Allocates memory for player, and 
    if it's not capable exits the main program */
    newPlayer = (Player *) malloc(sizeof(Player));
    if (newPlayer == NULL) {
        ALLOCATION_ERROR_MESSAGE();
        exit(EXIT_FAILURE);
    }
    
    /* Prompts the user for the player's informtations */
    PromptNewPlayer(newPlayer, *players);
    
    /* Initializes remaining player's fields */
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

/* FUNCTION NAME:ReadAIActions
*  DESCRIPTION: Reads AI strategy from a file and saves it into
*  "2 dimensional vector"*/
void ReadAIActions(FILE *ai_strategy, AIAction ***ai_actions) {
    int currentColumn = 0, currentRow = 0;
    char c;
    
    /* Allocates memory for a vector of pointers, in which pointer is going to point to the
    first position of a soon to be row of a "2 dimensional vector" and if it's not capable
    exits the main program */
    *ai_actions = (AIAction **) malloc(sizeof(AIAction *) * AIACTIONS_ROWS); // *ai_actions points to the refered vector
    if (*ai_actions == NULL) {
        ALLOCATION_ERROR_MESSAGE();
        
        exit(EXIT_FAILURE);
    }
    
    /* Allocates memory for all the elements of each row (for all rows),which
    in a "2 dimensional matrix" the number of elements of a row corresponds to its number of
    columns, and if its not capable to allocate the memory exits the main program */
    for (int i = 0; i < AIACTIONS_ROWS; i++) {
        (*ai_actions)[i] = (AIAction *) malloc(sizeof(AIAction) * AIACTIONS_COLUMNS); // (*ai_actions)[i] is the pointer to the the first element of each row
        if ((*ai_actions)[i] == NULL) {
            ALLOCATION_ERROR_MESSAGE();
            exit(EXIT_FAILURE);
        }
    }
    
    /* Search for all the characters in the AI strategy file */
    while ((c = fgetc(ai_strategy)) != EOF) {
        /* If the line is over increment the current row, reset 
        the current column and passes to the next iteration */
        if (c == '\n' || c == '\r') {
            currentRow += 1;
            currentColumn = 0;
            continue;
        }
        
        /* If it's not a valid value for an AI action exits the main program */
        if (c > '4' || c < '0') {
            ConfigurationFileError();
        }

        /* If the current row or the current column are greater than the dimension
        of the matrix, the matrix is complete */
        if (currentRow > AIACTIONS_ROWS || currentColumn > AIACTIONS_COLUMNS) {
            break;
        }
        
        /* Writes the AI action in the corresponding position in the matrix */
        (*ai_actions)[currentRow][currentColumn] = (AIAction) (c - '0');
        
        currentColumn += 1; // Increments the current column
    }
}

/* FUNCTION NAME:GameSettings
*  DESCRIPTION: Reads the game settings from two files and treats the data */
void GameSettings(char *config_file, char *ai, int *decks, Player_node **resultPlayers, AIAction ***ai_actions) {
    FILE* game_file = NULL;
    FILE* ai_strategy = NULL;
    Player_node *aux = NULL;
    Player_node *head = NULL;
    int players;
    char line[MAX_LINE] = { 0 };
    char *tmp = NULL;
    
    /* Opens the configuration file in read mode and if
    it's not capable exits the main program */
    game_file = fopen(config_file, "r");
    if(game_file == NULL){
        printf("Error opening configuration file!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Opens the AI strategy file in read mode and if
    it's not capable exits the main program */
    ai_strategy = fopen(ai, "r");
    if(ai_strategy == NULL){
        printf("Error opening AI strategy file!\n");
        exit(EXIT_FAILURE);
    }
    /* Reads AI strategy from the file and saves it
    in a "2 dimensional vector" */
    ReadAIActions(ai_strategy, ai_actions);
    
    /* Reads the first line in the game configuration
    file and if doesn't find it exits the main program */
    if (fgets(line, MAX_LINE, game_file) == NULL) {
        ConfigurationFileError();
    }
    
    /* Gets the first chunck till '-' appears from the read line
    and this chunck should be an integer (and corresponds to the number 
    of decks to be played with), and if it's not found or it's not an 
    integer exits the main program */
    tmp = strtok(line, CONFIG_SEPARATOR);
    if (sscanf(tmp, "%d", decks) == 0) {
        ConfigurationFileError();
    }
    
    /* Gets the next chunck till '-' appears from the read line
    and this chunck should be an integer (number of player), and if it's not found 
    or it's not an integer exits the main program */
    tmp = strtok(NULL, CONFIG_SEPARATOR);
    if (sscanf(tmp, "%d", &players) == 0) {
        ConfigurationFileError();
    }
	
	/* If the number of decks or the number of players are not 
	between the imposed boundaries exits the main program */
    if(*decks < MIN_DECKS || *decks > MAX_DECKS || players < MIN_PLAYERS || players > MAX_PLAYERS){
        printf("Not valid players or deck parameters!\n");
        exit(EXIT_FAILURE);
    }
    
    
    aux = head;
 
    /*  */
    for (int i = 0; i < players; i++) { // Searches for all players expexted
        /* Allocates memory for player, and 
	if it's not capable exits the main program */
        Player *newPlayer = (Player *) malloc(sizeof(Player));
        if (newPlayer == NULL) {
            ALLOCATION_ERROR_MESSAGE();
            exit(EXIT_FAILURE);
        }
        
        /* Reads players information from the game configuration file
        an saves it in newPlayer and if it's not capable exits the main program*/
        ReadGameSettingsPlayer(game_file, newPlayer);
        newPlayer->position = i; // Sets the player position on the table
        
        insert_player_node(&head, &aux, *newPlayer); // Inserts the player on the tail of the list if capable, else exits the main program
        
        free(newPlayer);
    }
    
    *resultPlayers = head; // Writes the head of the players' list in *resultPlayers
    
    /* Closes the game settings' files */
    fclose(game_file);
    fclose(ai_strategy);
    
}
