#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "cards.h"
#include "blackjack.h"
#include "deck.h"

void reads(char *buffer, int max) {
    fgets(buffer, max, stdin);
    buffer[strlen(buffer) - 1] = '\0';
}

void GetBankroll_GameResults(Player* house, Player_node **head) {
    Player_node *walk = *head;
    int i = 0;
    
    while(walk != NULL) {
        if(walk->player.surrender == true) {
        	walk->player.surrender = false;
        } else if (Bust(walk->player)) {
            walk->player.games_result.lost += 1;
        } else if (house->score > 21) {
            walk->player.money += 2 * (walk->player.bet) + 0.5 * (walk->player.bet) * (walk->player.score == 21 && walk->player.hand_size == 2);
            walk->player.games_result.won += 1;
        } else if (house->score == walk->player.score) {

            if (house->score == 21) {

                if (walk->player.hand_size == house->hand_size || (walk->player.hand_size != 2 && house->hand_size != 2)) {
                    walk->player.money += (walk->player.bet);
                    walk->player.games_result.tied+=1;
                } else if (walk->player.hand_size == 2 && house->hand_size != 2) {
                    walk->player.money += 2.5 * (walk->player.bet);
                    walk->player.games_result.won += 1;
                } else {
                    walk->player.games_result.lost += 1;
                }

            } else {
                walk->player.money += (walk->player.bet);
                walk->player.games_result.tied += 1;
            }

        } else if (walk->player.score != 21 || (walk->player.score == 21 && walk->player.hand_size != 2)) {
            walk->player.money += 2 * (walk->player.bet) * (walk->player.score > house->score);

            if (walk->player.score > house->score) {
                walk->player.games_result.won +=1;
            } else {
                walk->player.games_result.lost +=1;
            }

        } else {
            walk->player.money += 2.5 * (walk->player.bet);
            walk->player.games_result.won += 1;
        }
        
        if (walk->player.bet > walk->player.money) {
            walk->player.bet = walk->player.money;
        }

        walk = walk->next;
        i += 1;
    }
}

void FinishTurn(Card_node **deck_head, int numberOfDecks, Player* house, Player_node **head) {
    PlayHouse(house, deck_head, numberOfDecks);
    GetBankroll_GameResults(house, head);
}

void PlayHouse(Player *house, Card_node **deck_head, int numberOfDecks)
{
    int numberOfAces = 0; // number of aces in house hand
    Card_node *card_aux = NULL;
    house->hand_size = 2; // shows the card face down

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
            push_card_node(&(house->cards), NextCard(deck_head, numberOfDecks));
            house->hand_size += 1; // House Hits!
        }
    } while (house->score < 16);
}

Player_node* Hit(Card_node** deck_head, Player_node* current_player, int numberOfDecks) {
    Card_node *n = NextCard(deck_head, numberOfDecks);
    push_card_node(&(current_player->player.cards), n);
    current_player->player.hand_size += 1;

	GetScore(&(current_player->player));

	if (Bust(current_player->player)) {
		return Stand(current_player);
	}

	return current_player;
}

Player_node* Stand(Player_node* current_player) {
	return current_player->next;
}

Player_node* Double(Card_node **deck_head, int numberOfDecks, Player *house, Player_node *current_player) {
    float bet = (current_player->player).bet;
    
    if (bet <= current_player->player.money && current_player->player.hand_size == BLACKJACK_INITIAL_CARDS) {
        (current_player->player).money -= bet;
        (current_player->player).bet *= 2;
        Hit(deck_head, current_player, numberOfDecks);
        
        return current_player->next;
    }
    
    return current_player;
}

Player_node* Surrender(Player_node *current_player) {
    if (current_player->player.hand_size > BLACKJACK_INITIAL_CARDS) {
        return current_player;
    }
    
	current_player->player.surrender = true;
	current_player->player.money += 0.5 * (current_player->player.bet);
	current_player->player.games_result.lost += 1;
    
	return current_player->next;
}

void Bet(Player_node *head) {
	Player_node *tmp = NULL;
    char *buf = (char *) malloc((MAX_NAME + 1) * sizeof(char));
    char *buf2 = (char *) malloc((MAX_NAME + 1) * sizeof(char));
	float bet = 0;
    int err = 0;
    bool playerSelected = false;

	do {
		tmp = head;
        
		printf("Player name: ");
        
        reads(buf, MAX_NAME + 1);
        
        while(tmp != NULL) {
            if (strcmp(tmp->player.name, buf) == 0) {
                playerSelected = true;
                
                do {
                    printf("New bet value: ");

                    fgets(buf2, MAX_NAME + 1, stdin);
                    err = sscanf(buf2, "%f", &bet);

                    if(err != 1 || bet < MIN_BET_VALUE || bet > tmp->player.money)
                        printf("Bet value not valid!\n");
                    
                } while(err != 1 || bet < MIN_BET_VALUE || bet > tmp->player.money);

                break;
            }
            
            tmp = tmp->next;
        }
        
        if (!playerSelected) {
            printf("There's no player with such name.\n");
        }
	} while(!playerSelected);

	tmp->player.bet = bet;
    
    free(buf);
    free(buf2);
}

int DealCards(Card_node** deck_head, Player_node* head, Player* house, int numberOfDecks, int numberOfCardsToDeal) {
    int cardsDealt = 0;

    if(house != NULL) {
        Player_node *walk = head;

		for(int i=0; i < numberOfCardsToDeal; i++) {

			while (walk != NULL) {
				push_card_node(&(walk->player.cards), NextCard(deck_head, numberOfDecks));
				walk->player.hand_size += 1;
                		cardsDealt += 1;
                		
				walk = walk->next;
			}

			push_card_node(&(house->cards), NextCard(deck_head, numberOfDecks));
			house->hand_size = 1;
            		
            walk = head;
		}
	}

    GetPlayerListScore(head);

    return cardsDealt;
}

Card_node *NextCard(Card_node **deck_head, int numberOfDecks) {
    if(empty(*deck_head)){
        *deck_head = DeckMaker(numberOfDecks);
        ShuffleCards(deck_head, numberOfDecks, 1); //TODO: Times really needed??
    }

    return take_card_node(deck_head, 0);
}
