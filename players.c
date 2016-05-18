#include <stdlib.h>
#include <stdio.h>
#include "players.h"

const short CARD_POINTS[] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

void GetPlayerListScore(Player_node *head) {
    Player_node *walk = head;

    while (walk != NULL) {

        GetScore(&(walk->player));

        walk = walk->next;
    }
}

void GetScore(Player* player) {
    short score = 0, aces = 0;
    Card_node* cardnode_ptr;


    cardnode_ptr = player->cards;

    // calculate the score excluding the aces
    while(cardnode_ptr != NULL) {

        if (cardnode_ptr->card.id != 13) {
            score += CARD_POINTS[cardnode_ptr->card.id];
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

void add_player(Player_node** head, Player data) {
    Player_node *new_node = NULL;

	new_node = create_player_node(data);

	if(empty(new_node)){
		ERROR_MESSAGE();
		exit(EXIT_FAILURE);
	}

    new_node->next = *head;
    new_node->player = data;
    *head = new_node;
}

void insert_player_node(Player_node **head, Player_node **tail, Player data) {
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
}

Player_node *take_player_node(Player_node** head, int position) {
    Player_node *tmp;
    Player_node *curr=*head;
    
    if(!empty(curr)) {
        if(position < 0)
            return NULL;
        else if(position == 0) {
            *head = curr->next;
            curr->next = NULL;
            return curr;
        }
        
        for(int i=1; i < position && curr != NULL; i++)
            curr=curr->next;
        
        if(empty(curr)) {
            return NULL;
        }
        
        tmp = curr->next;
        curr->next = curr->next->next;
        tmp->next = NULL;
        
        return tmp;
    }
    
    return NULL;
}

void join_player_node(Player_node** head, Player_node* jointo, int position) {
    Player_node *curr = *head;
    if(!empty(curr)) {
        
        if(position < 0) {
            return;
        }
        
        if(position == 0){
            jointo->next = *head;
            *head = jointo;
            return;
        }
        
        for(int i=1; i < position && curr != NULL; i++)
            curr=curr->next;
        
        if(empty(curr)) {
            return;
        }
        
        jointo->next = curr->next;
        curr->next = jointo;
    } else {
        *head = jointo;
    }
}

void insert_sorted_player_node(Player_node **head, Player data) {
    Player_node *tmp = NULL;
    Player_node *curr = *head;
    Player_node *new_node = create_player_node();
    
    new_node->player = data;
    
    if (empty(*head)) {
        free(new_node);
        insert_player_node(head, head, data);
    } else if((*head)->player.position > data.position) {
        new_node->next = *head;
        *head = new_node;
    } else {
        while (curr != NULL) {
            tmp = curr;
            curr = curr->next;
            
            if (curr == NULL || curr->player.position > data.position) {
                new_node->next = curr;
                tmp->next = new_node;
                break;
            }
        }
    }
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
