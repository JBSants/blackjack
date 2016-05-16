#include <stdlib.h>
#include <stdio.h>
#include "players.h"

void GetPlayerListScore(Player_node *head) {
    Player_node *walk = head;

    while (walk != NULL) {

        GetScore(&(walk->player));

        walk = walk->next;
    }
}

void GetScore(Player* player) {
    const short card_points[] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

    short score = 0, aces = 0;
    Card_node* cardnode_ptr;


    cardnode_ptr = player->cards;

    // calculate the score excluding the aces
    while(cardnode_ptr != NULL) {

        if (cardnode_ptr->card.id != 13) {
            score += card_points[cardnode_ptr->card.id];
        } else {
            aces++;
        }
        cardnode_ptr = cardnode_ptr->next;
    }

    // calculates the aces' points and sums it to the rest of the score
    for(;aces>0;aces--) {
        if ((score + 11) <= 22 - aces) {
            score += 11;
        } else {
            score += 1;
        }
    }
    player->score = score;
}

Stat add_player(Player_node** head, Player data) {
    Player_node *new_node = NULL;

	new_node = create_player_node(data);

	if(empty(new_node)){
		ERROR_MESSAGE();
		exit(EXIT_FAILURE);
	}

    new_node->next = *head;
    new_node->player = data;
    *head = new_node;

	return ACT_STAT;
}

Stat insert_player(Player_node **head, Player_node **tail, Player data) {
    Player_node *new_node = create_player_node();
    
    if (new_node == NULL) {
        ERROR_MESSAGE();
        exit(EXIT_FAILURE);
    }
    
    if (*tail != NULL) {
        (*tail)->next = new_node;
    }
    
    new_node->next = NULL;
    new_node->player = data;
    
    *tail = new_node;
    
    if (*head == NULL) {
        *head = *tail;
    }
    
    return ACT_STAT;
}

Player_node *create_player_node() {
	Player_node *new_node = malloc(sizeof(Player_node));

	if(empty(new_node)) {
    	ERROR_MESSAGE();
    	exit(EXIT_FAILURE);
    }

    return new_node;
}
void erase_player_list(Player_node* head) {
	Player_node* tmp;

	while((tmp = head) != NULL){
		head = head->next;
		free(tmp);
	}
}
