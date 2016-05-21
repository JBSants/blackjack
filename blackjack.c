#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "cards.h"
#include "blackjack.h"
#include "deck.h"
#include "ai.h"


/**
 * reads: Reads a string from stdin, removing the \n from the end
 * \param buffer
 * \param max
 */
void reads(char *buffer, int max) {
    fgets(buffer, max, stdin);
    buffer[strlen(buffer) - 1] = '\0'; // Removes \n from the end
}

/**
 * GetBankroll_GameResults: Determines turn result, updates players money, stats and house balance
 * \param house house player
 * \param head players list
 */
void GetBankroll_GameResults(Player* house, Player_node **head) {
    Player_node *walk = *head; // Auxiliary variable used to walk the list
    int i = 0; // Iterator
    
    /* Walks players list and updates each players money and stats */
    while(walk != NULL) {
        
        if(walk->player.surrender == true) {
        	walk->player.surrender = false;
            house->money += 0.5 * walk->player.bet;
        } else if (Bust(walk->player)) {
            walk->player.games_result.lost += 1;
            house->money += walk->player.bet;
        } else if (house->score > BLACKJACK_MAX_SCORE) {
            walk->player.money += 2 * (walk->player.bet) + 0.5 * (walk->player.bet) * Blackjack(walk->player); // adds bet money, and half if player has blackjack
            walk->player.games_result.won += 1;
            house->money -= (walk->player.bet) + 0.5 * (walk->player.bet) * Blackjack(walk->player); // removes bet money from balance, and half if player has blackjack
        } else if (house->score == walk->player.score) {

            if (house->score == BLACKJACK_MAX_SCORE) {

                if (walk->player.hand_size == house->hand_size || (walk->player.hand_size != 2 && house->hand_size != 2)) {
                    walk->player.money += (walk->player.bet);
                    walk->player.games_result.tied += 1;
                } else if (walk->player.hand_size == 2 && house->hand_size != 2) {
                    walk->player.money += 2.5 * (walk->player.bet);
                    walk->player.games_result.won += 1;
                    house->money -= 1.5 * (walk->player.bet);
                } else {
                    house->money += walk->player.bet;
                    walk->player.games_result.lost += 1;
                }

            } else {
                walk->player.money += (walk->player.bet);
                walk->player.games_result.tied += 1;
            }

        } else if (walk->player.score != BLACKJACK_MAX_SCORE || (walk->player.score == BLACKJACK_MAX_SCORE && walk->player.hand_size != 2)) {
            walk->player.money += 2 * (walk->player.bet) * (walk->player.score > house->score);

            if (walk->player.score > house->score) {
                walk->player.games_result.won +=1;
                house->money -= walk->player.bet;
            } else {
                walk->player.games_result.lost +=1;
                house->money += walk->player.bet;
            }

        } else {
            walk->player.money += 2.5 * (walk->player.bet);
            walk->player.games_result.won += 1;
            house->money -= 1.5 * (walk->player.bet);
        }

        walk = walk->next;
        i += 1;
    }
}

/**
 * FinishTurn: Finishes current turn, playing house turn and getting turn results.
 * \param deck_head pointer to the head of the deck
 * \param numberOfDecks
 * \param house house player
 * \param head players list
 * \param hilo hilo current value
 * \param cardsDealt number of dealt cards
 */
void FinishTurn(Card_node **deck_head, int numberOfDecks, Player* house, Player_node **head, int *hilo, int *cardsDealt) {
    PlayHouse(house, deck_head, numberOfDecks, hilo, cardsDealt);
    GetBankroll_GameResults(house, head);
}


/**
 * PlayHouse: Plays house turn.
 * \param house house player
 * \param deck_head pointer to the head of the deck
 * \param numberOfDecks
 * \param hilo hilo current value
 * \param cardsDealt number of dealt cards
 */
void PlayHouse(Player *house, Card_node **deck_head, int numberOfDecks, int *hilo, int *cardsDealt)
{
    int numberOfAces = 0; // Number of aces in house hand
    Card_node *card_aux = NULL; // Auxiliary variable for walking house card list
    house->hand_size = 2; // Shows the card face down

    /* At least once and while house has less than 16 points
     * hits a card only if house points is less than 16, or is 17
     * and house has one ace
     */
    do {
        numberOfAces = 0;
        card_aux = house->cards;

        GetScore(house);

        /* Determines the number of aces in house hand */
        while (card_aux != NULL) {
            if (IsAce(card_aux->card)) {
                numberOfAces += 1;
            }
            card_aux = card_aux->next;
        }

        /* If house points is less than 16, or is 17 and house has one ace */
        if (house->score < 16 || (numberOfAces == 1 && house->score == 17)) {
            push_card_node(&(house->cards), NextCard(deck_head, numberOfDecks, hilo, cardsDealt));
            house->hand_size += 1; // House Hits!
        }
    } while (house->score < 16);
}

/**
 * NewTurn: Begins a new turn. Resets players score and house score, removes non active
 * players, updates ai bet based on the hilo method and deals new cards.
 * \param deck_head pointer to the head of the deck
 * \param numberOfDecks
 * \param players players list
 * \param removedPlayers non active players list
 * \param currentPlayerNode pointer to the current player node
 * \param house house player
 * \param hilo hilo current value
 * \param cardsDealt number of dealt cards
 */
void NewTurn(Card_node **deck_head, int numberOfDecks, Player_node **players, Player_node **removedPlayers, Player_node **currentPlayerNode, Player *house, int *hilo, int *cardsDealt) {
    Player_node *player_node_aux = *players; // Auxiliary variable used for walking players list
    Player_node *tmp_player = NULL; // Auxiliary variable
    int i = 0; // Iterator
    
    /* Walks each player, resetting it for the new turn, updates ai bet and removes non active players */
    while (player_node_aux != NULL) {
        Card_node **player_cards = &(player_node_aux->player.cards);
        
        /* Frees cards */
        while (*player_cards != NULL) {
            free(pop_card(player_cards));
        }
        
        player_node_aux->player.score = 0;
        player_node_aux->player.hand_size = 0;
        
        /* If player is ai, update it's bet using hilo method */
        if (player_node_aux->player.ai) {
            int decksNotDealt = (numberOfDecks - (*cardsDealt / DECK_MAX_CARDS));
            int hiloValue = 0;
            
            if (decksNotDealt > 0) {
                hiloValue = *hilo / decksNotDealt;
                
                player_node_aux->player.bet = 2 * hiloValue * player_node_aux->player.initialBet;
                
                /* Checks if bet is less than initial bet (hilo bet is 0) */
                if (player_node_aux->player.bet < player_node_aux->player.initialBet) {
                    
                    /* If so, Checks if bet is correct, and if the ai player has money for it and fixes it */
                    if (player_node_aux->player.bet > player_node_aux->player.money || player_node_aux->player.initialBet > player_node_aux->player.money) {
                        player_node_aux->player.bet = player_node_aux->player.money;
                    } else {
                        player_node_aux->player.bet = player_node_aux->player.initialBet;
                    }
                }
            }
        }
        
        /* Updates player bet */
        if (player_node_aux->player.bet > player_node_aux->player.money) {
            player_node_aux->player.bet = player_node_aux->player.money;
        }
        
        /* If player has no more money removes it */
        if (player_node_aux->player.money <= 0) {
            tmp_player = player_node_aux->next;
            join_player_node(removedPlayers, take_player_node(players, i), 0); // Remove !
            player_node_aux = tmp_player;
        } else {
            player_node_aux->player.money -= player_node_aux->player.bet;
            player_node_aux = player_node_aux->next;
        }
        
        i += 1;
    }
    
    house->score = 0;
    house->hand_size = 0;
    erase_card_list(house->cards);
    house->cards = NULL;
    
    DealCards(deck_head, *players, house, numberOfDecks, BLACKJACK_INITIAL_CARDS, hilo, cardsDealt);
    
    *currentPlayerNode = *players; // Resets current player to the first player
}

/**
 * Hit: Hits. If bust stands
 * \param deck_head pointer to the head of the deck
 * \param numberOfDecks
 * \param house house player
 * \param current_player pointer to the current player node
 * \param hilo hilo current value
 * \param cardsDealt number of dealt cards
 */
Player_node* Hit(Card_node** deck_head, Player_node* current_player, int numberOfDecks, int *hilo, int *cardsDealt) {
    Card_node *n = NextCard(deck_head, numberOfDecks, hilo, cardsDealt); // Gets deck next card
    
    push_card_node(&(current_player->player.cards), n); // Adds it
    current_player->player.hand_size += 1;

	GetScore(&(current_player->player));

    /* Checks bust, if so stands! */
	if (Bust(current_player->player)) {
		return Stand(current_player);
	}

	return current_player;
}

/**
 * Stand: Stands
 * \param current_player pointer to the current player node
 */
Player_node* Stand(Player_node* current_player) {
	return current_player->next;
}

/**
 * Double: Doubles player bet
 * \param deck_head pointer to the head of the deck
 * \param numberOfDecks
 * \param house house player
 * \param current_player pointer to the current player node
 * \param hilo hilo current value
 * \param cardsDealt number of dealt cards
 */
Player_node* Double(Card_node **deck_head, int numberOfDecks, Player *house, Player_node *current_player, int *hilo, int *cardsDealt) {
    float bet = (current_player->player).bet; // Current player bet
    
    /* Checks if double is allowed, if so doubles it! */
    if (bet <= current_player->player.money && current_player->player.hand_size == BLACKJACK_INITIAL_CARDS) {
        (current_player->player).money -= bet;
        (current_player->player).bet += bet;
        Hit(deck_head, current_player, numberOfDecks, hilo, cardsDealt);
        
        return current_player->next;
    }
    
    return current_player;
}

/**
 * Surrender: Current player surrenders
 * \param current_player pointer to the current player node
 */
Player_node* Surrender(Player_node *current_player) {
    /* Checks if surrender is allowed */
    if (current_player->player.hand_size > BLACKJACK_INITIAL_CARDS) {
        return current_player;
    }
    
    /* Surrenders! */
	current_player->player.surrender = true;
	current_player->player.money += 0.5 * (current_player->player.bet);
	current_player->player.games_result.lost += 1;
    
	return current_player->next;
}

/**
 * Bet: Updates a player bet
 * \param head players list
 */
void Bet(Player_node *head) {
	Player_node *tmp = NULL; // Auxiliary variable
    char *buf = (char *) malloc((MAX_NAME + 1) * sizeof(char)); // Buffer for storing player name and bet
	float bet = 0; // New bet
    int err = 0; // Error check variable
    bool playerSelected = false; // True if player has been selected

	do {
		tmp = head;
        
		printf("Player name: ");
        
        reads(buf, MAX_NAME + 1);
        
        /* Checks if string isn't empty */
        if (strlen(buf) <= 0) {
            break;
        }
        
        /* Tries to find selected player, if so playerSelected becomes true */
        while(tmp != NULL) {
            /* Compares current player name with buf, if equal and player is not ai, read new bet! */
            if (strcmp(tmp->player.name, buf) == 0 && !tmp->player.ai) {
                playerSelected = true;
                
                do {
                    printf("New bet value: ");

                    fgets(buf, MAX_NAME + 1, stdin);
                    err = sscanf(buf, "%f", &bet);

                    if(err != 1 || bet < MIN_BET_VALUE || bet > tmp->player.money)
                        printf("Bet value not valid!\n"); // Verifies new bet
                    
                } while(err != 1 || bet < MIN_BET_VALUE || bet > tmp->player.money);

                break;
            }
            
            tmp = tmp->next;
        }
        
        /* If no player error ! */
        if (!playerSelected) {
            printf("There's no player with such name.\n");
        }
	} while(!playerSelected);

    /* Updates bet if valid */
    if (bet > 0) {
        tmp->player.bet = bet;
    }
    
    free(buf);
}

/**
 * DealCards: Deals cards to each player and the house
 * \param deck_head pointer to the head of the deck
 * \param head players list
 * \param house house player
 * \param numberOfDecks
 * \param numberOfCardsToDeal
 * \param hilo hilo current value
 * \param cards_dealt number of dealt cards
 */
void DealCards(Card_node** deck_head, Player_node* head, Player* house, int numberOfDecks, int numberOfCardsToDeal, int *hilo, int *cardsDealt) {
    Player_node *walk = NULL;
    
    /* Checks errors */
    if(house != NULL && !empty(head)) {
        walk = head;
        
        /* Deals number of cards to each player and the house */
		for(int i = 0; i < numberOfCardsToDeal; i++) {

            /* Deals a card to each player */
			while (walk != NULL) {
				push_card_node(&(walk->player.cards), NextCard(deck_head, numberOfDecks, hilo, cardsDealt)); // Deals a card to the player
				walk->player.hand_size += 1;
                
				walk = walk->next;
			}

			push_card_node(&(house->cards), NextCard(deck_head, numberOfDecks, hilo, cardsDealt)); // Deals a card to the house
			house->hand_size = 1; // House hand_size is only one because it has a card faced down
            		
            walk = head;
		}
	}

    GetPlayerListScore(head); // Updates players score
}

/**
 * WriteMoneyAndStatsToFile: Writes statistics file
 * \param players players list
 * \param house house player
 * \param turn_ended true if turn has ended
 */
void WriteMoneyAndStatsToFile(Player_node *players, Player *house, bool turn_ended) {
    FILE *statsFile = NULL; // Pointer to be used for stats file access
    
    /* Opens the statistics file */
    statsFile = fopen(STATISTICS_FILE_NAME, "w");
    
    /* Checks if the file was opened */
    if (statsFile == NULL) {
        printf("Error creating statistics file!");
        
        exit(EXIT_FAILURE);
    }
    
    /* Prints some information */
    fprintf(statsFile, "-- Statistics -- \n\n");
    fprintf(statsFile, "[Player Name] ([Type]): [Games Won] | [Games Tied] | [Games Lost] (Money: [Final Money])\n\n");
    
    /* Writes statstics for each player */
    while(players != NULL) {
        
        /**
          * Checks if we are in the middle of a turn
          * and the player is active, if so adds bet
          *
          */
        if (!turn_ended && players->player.money >= 0) {
            players->player.money += players->player.bet;
        }
        
        fprintf(statsFile, "%s (%s): %d | %d | %d (Money: %.2f)\n", players->player.name, players->player.ai ? AI : NOT_AI, players->player.games_result.won, players->player.games_result.tied, players->player.games_result.lost, players->player.money);
        
        players = players->next;
    }
    
    /* Writes the house final balance */
    fprintf(statsFile, "House Money: %.2f", house->money);
    
    /* Closes the statistics file */
    fclose(statsFile);
}
