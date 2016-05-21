#include "cards.h"
#include <stdio.h>
#include <stdlib.h>

/** 
 * add_card: Adds a card to the head of the list 
 * \param head pointer to the head of a list passed by reference
 * \param data
 */
void add_card(Card_node** head, Card data) {
	
	/* Allocates memory for a new node to hold the card, and 
	if it's not capable, exits the main program */
	Card_node* new_node = create_card_node();
	
	/* Inserts a node in the head
        with the specified data */
	new_node->next = *head;
	new_node->card = data;
	*head = new_node;

}

/**
 * insert_card: Inserts a card in a specified position on the list
 * \param head pointer to the head of a list passed by reference
 * \param data
 * \param position position in the list (starting on 0) to insert the node yet to create
 */
void insert_card(Card_node** head, Card data, int position) {
    Card_node* curr = *head;
    
    /* If the list isn't empty (not pointing to NULL) */
    if (!empty(curr)) {
        
        /* There aren't negative positions on the list, so return with no actions done */
        if(position < 0)
            return;

	/* Allocates memory for a new node to hold the card, and 
	if it's not capable, exits the main program */
 	Card_node* new_node = create_card_node();
	
	/* Inserts on the first position of the list (head) */
        if(position == 0) {
            new_node->card=data;
            new_node->next=*head;
            *head=new_node;
        }

        /* Search the node in the position specified, until the end of the list */
        for(int i = 1; i < position && curr!=NULL; i++) {
            curr=curr->next;
        }
	
	/* If the position doesn't exist (more than the size of the list) 
	returns with the list unchanged */
        if(empty(curr)) {
            free(new_node);
            return;
        }

        /* Inserts the node in the specified 
        place with the specified data */
        new_node->next=curr->next;
        new_node->card=data;
        curr->next=new_node;
    }
}

/**
 * take_card_node: Takes the node (if possible) from the selected 
 * position of the list and returns a pointer to it
 * \param head pointer to the head of a list passed by reference
 * \param position position in the list to take the node (starting on 0)
 */
Card_node* take_card_node(Card_node** head, int position) {
    Card_node* tmp = NULL;
    Card_node* curr = *head;

	/* If the list isn't empty (not pointing to NULL) */
	if(!empty(curr)) {
        	/* There aren't negative positions on the list, so return without no actions done */
        	if(position < 0)
            		return NULL;
        	
        	/* Takes a node from the first position of the list (head) */
        	else if(position == 0) {
            		*head = curr->next;
            		curr->next = NULL;
            		return curr;
        	}
		
		/* Search the node in the position specified, until the end of the list */
        	for(int i=1; i < position && curr != NULL; i++)
        		curr=curr->next;
		
		/* If the position doesn't exist (more than the size of the list) 
		returns with the list unchanged */
        	if(empty(curr)) {
            		return NULL;
        	}
        	
        	/* Takes node from the specified position */
        	tmp = curr->next;
        
            if (tmp != NULL) {
                curr->next = curr->next->next;
                tmp->next = NULL;
            }

        	return tmp; //Returns a pointer to the taken node
    	}

    return NULL;
}

/** join_card_node: Joins a node (if possible) in the selected,
 *  else doesn't do anything
 * \param head head pointer to the head of a list passed by reference
 * \param jointo node to join the list
 * \param position in the list to join the node (starting on 0)
 */
void join_card_node(Card_node** head, Card_node* jointo, int position) {
    Card_node* curr = *head;
	
	if (!empty(jointo)){ //If there is a node to join
		/* If the list isn't empty (not pointing to NULL) */
		if(!empty(curr)) {
			/* There aren't negative positions on the list, so return without no actions done */
			if(position < 0) {
				return;
			}
        	
        		/* Joins a node in the first position of the list (head) */
        		if(position == 0) {
				jointo->next = *head;
				*head = jointo;
				return;
			}
		
			/* Search the node in the position specified, until the end of the list */
			for (int i=1; i < position && curr != NULL; i++)
				curr=curr->next;
		
			/* If the position doesn't exist (more than the size of the list) 
			returns with the list unchanged */
			if(empty(curr)) {
				return;
			}
		
			/* Joins node to the list's specified position */
			jointo->next = curr->next;
			curr->next = jointo;
	
		} else { //if the list is empty the node to join is in the head (independent of the position)
			*head = jointo;
		}
	}
}

/** create_card_node: Creates a node */
Card_node* create_card_node() {
	
	/* Allocates memory for a new node and 
	if it's not capable, exits the main program */
	Card_node* new_node = (Card_node *) malloc(sizeof(Card_node));
	if(empty(new_node)) {
        	ALLOCATION_ERROR_MESSAGE();
        	exit(EXIT_FAILURE);
    	}

	return new_node; //returns a pointer to the created node
}

/** erase_card_list: Erases a list 
 * \param head head pointer to the head of a list
 */
void erase_card_list(Card_node* head) {
    Card_node* tmp;
    
    /* Removes each list's element */
    while((tmp = head) != NULL) {
        head = head->next;
        free(tmp);
    }
}

///Stack

/* push_card: Performs a push into the stack of a card */
void push_card(Card_node** head, Card data) {
       	
       	/* Allocates memory for a new node to hold the card, and 
	if it's not capable, exits the main program */
        Card_node* new_node = create_card_node();
	
        /* Push of the card into the stack */
        new_node->card = data;
        push_card_node(head, new_node);
}


/* push_card_node: Performs a push into the stack of a node */
void push_card_node(Card_node** head, Card_node* node){
	
	/* If there is a node (node pointer doesn't point to NULL)
	push the node into the stack */
	if(!empty(node)) {
		node->next = *head;
        *head = node;
	}
}

/* pop_card: Performs a pop onto the stack */
Card_node *pop_card(Card_node** head) {
	Card_node* tmp = NULL;
	
	/* If the stack isn't empty (not pointing to NULL), pop */
	if(!empty(*head)) {
		tmp = *head;
		(*head) = tmp->next;
		tmp->next = NULL;
	}

	return tmp; //returns a pointer to the popped node
}

/** CountAces: Counts the aces in a card's list/stack 
 * \param head pointer to the head of a list
 */
int CountAces(Card_node *head) {
    Card_node *walk = head;
    int result = 0;
    
    /* If the list/stack is not empty(head does not point 
    to NULL), walks through it in search for acesand counts them */
    if (walk != NULL) {
        while (walk != NULL) {
            if (IsAce(walk->card)) {
                result += 1;
            }
            
            walk = walk->next;
        }
    }
    
    return result; //returns the number of aces in the list/stack
}
