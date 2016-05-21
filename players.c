#include <stdlib.h>
#include <stdio.h>
#include "players.h"

/* FUNCTION NAME:GetPlayerListScore
*  DESCRIPTION: Calculates the score of all players in the list */
void GetPlayerListScore(Player_node *head) {
	Player_node *walk = head;

	/* Walks through all players in the list
	and calculates their score*/
	while (walk != NULL) {
		GetScore(&(walk->player));
		walk = walk->next;
	}
}

/* FUNCTION NAME:GetScore
*  DESCRIPTION: Calculates the player's score */
void GetScore(Player* player) {
    short score = 0, aces = 0;
    Card_node *cardnode_ptr = NULL;
    const short CARD_POINTS[] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};


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
    for(; aces > 0; aces--) {
        if ((score + MAX_ACE_POINTS) <= 22 - aces) {
            score += MAX_ACE_POINTS;
        } else {
            score += 1;
        }
    }
    
    player->score = score;
}

/* FUNCTION NAME:add_player
*  DESCRIPTION: Adds a player to the head of the players' list */
void add_player(Player_node** head, Player data) {
	Player_node *new_node = NULL;
	
	/* Creates a node if possible, if not
	exits the main program */
	new_node = create_player_node(data);

	/* Inserts a node in the head
        with the specified data */
	new_node->next = *head;
	new_node->player = data;
	*head = new_node;
}

/* FUNCTION NAME:insert_player_node
*  DESCRIPTION: Adds a card to the tail of the list */
void insert_player_node(Player_node **head, Player_node **tail, Player data) {
	
	/* Allocates memory for a new node to hold the card, and 
	if it's not capable, exits the main program */
	Player_node *new_node = create_player_node();
	
	/* If the list has a tail (list isn't empty) */
	if (*tail != NULL) 
        	(*tail)->next = new_node;
    
    	/* Adds a node in the tail 
    	with the specified data */
    	new_node->next = NULL;
    	new_node->player = data;
    	*tail = new_node;
    	
    	/* If the list is empty updates the head too */
    	if (*head == NULL) {
        	*head = *tail;
    	}
}

/* FUNCTION NAME:take_player_node
*  DESCRIPTION: Takes the node (if possible) from the selected 
*  position of the list and returns a pointer to it*/
Player_node *take_player_node(Player_node** head, int position) {
    Player_node *tmp = NULL;
    Player_node *curr = *head;
    
    /* If the list isn't empty (not pointing to NULL) */
    if(!empty(curr)) {
        /* There aren't negative positions on the list, so return without no actions done */
        if(position < 0)
            return NULL;
        
        /* Takes a node from the first position of the list (head) */
        if(position == 0) {
            *head = curr->next;
            curr->next = NULL;
            return curr;
        }
        
        /* Search the node in the position specified, until the end of the list */
        for(int i = 1; i < position && curr != NULL; i++)
            curr = curr->next;
        
        /* If the position doesn't exist (more than the size of the list) 
	returns with the list unchanged */
        if(empty(curr)) {
            return NULL;
        }
        
        tmp = curr->next; //Gets the node to remove
        
        /* If the node exists removes it from the list */
        if (tmp != NULL) {
            curr->next = curr->next->next;
            tmp->next = NULL;
        }
        
        return tmp; //Returns pointer to the taken node
    }
    
    return NULL;
}

/* FUNCTION NAME:join_player_node
*  DESCRIPTION: Joins a node (if possible) in the 
selected position, else doesn't do anything*/
void join_player_node(Player_node** head, Player_node* jointo, int position) {
    Player_node *curr = *head;
   
    if (!empty(jointo)) { //If there is a node to join
    	/* If the list isn't empty (not pointing to NULL) */
        if(!empty(curr)) {
            /* There aren't negative positions on the list, so return without no actions done */
            if(position < 0) {
                return;
            }
            
            /* Joins a node in the first position of the list (head) */
            if(position == 0){
                jointo->next = *head;
                *head = jointo;
                return;
            }
            
            /* Search the node in the position specified, until the end of the list */
            for(int i = 1; i < position && curr != NULL; i++)
                curr = curr->next;
            
            /* If the position doesn't exist (more than the size of the list) 
	    returns with the list unchanged */
            if(empty(curr)) {
                return;
            }
            
            /* Joins node to the list's specified position */
            jointo->next = curr->next;
            curr->next = jointo;
        } else {//if the list is empty the node to join is in the head (independent of the position)
            *head = jointo;
        }
    }
}

/* FUNCTION NAME:insert_sorted_player_node
*  DESCRIPTION: */
void insert_sorted_player_node(Player_node **head, Player data) {
    Player_node *tmp = NULL;
    Player_node *curr = *head;
    
    /* Allocates memory for node, and 
    if it's not capable exits the main program */
    Player_node *new_node = create_player_node();
    
    new_node->player = data; // Saves the player data in the corresponding node field
    
    /* If the list is empty (head pointing to NULL) inserts
    the player in the head and if it's not capable exits the main program*/
    if (empty(*head)) {
        free(new_node);
        insert_player_node(head, head, data);
    /* If the element in the head has a bigger position than the position
    of the player to insert in the list, inserts the player in the head
    (position in ascending order)*/
    } else if((*head)->player.position > data.position) {
        new_node->next = *head;
        *head = new_node;
    } else { // The position of the player is bigger or equal to than the one in the head
       
       /* Walks through the list until it finds the place where the player position
       is in ascending order with the rest of the list */
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
/* FUNCTION NAME:create_player_node
*  DESCRIPTION: Creates a node */
Player_node *create_player_node() {
	
	/*Allocates memory for a new node and 
	if it's not capable, exits the main program*/
	Player_node *new_node = malloc(sizeof(Player_node));
	if(empty(new_node)) {
    		ALLOCATION_ERROR_MESSAGE();
    		exit(EXIT_FAILURE);
    }

    return new_node; //returns a pointer to the created node
}

/* FUNCTION NAME:erase_player_list
*  DESCRIPTION: Erases a list */
void erase_player_list(Player_node* head) {
	Player_node *tmp;
	
	/*Removes each list's element*/
	while((tmp = head) != NULL){
		head = head->next;
		free(tmp);
	}
}
