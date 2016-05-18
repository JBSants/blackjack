#include "cards.h"
#include <stdio.h>
#include <stdlib.h>

void add_card(Card_node** head, Card data) {
	Card_node* new_node = create_card_node();
    
	if (new_node == NULL) {
        ERROR_MESSAGE();
        exit(EXIT_FAILURE);
    }

	new_node->next = *head;
	new_node->card = data;
	*head = new_node;

}

/* FUNCTION NAME:inser_card
*  DESCRIPTION: Inserts a card in a specified position on the list*/
void insert_card(Card_node** head, Card data, int position) {
    Card_node* curr = *head;
    
    /*If the list isn't empty (not pointing to NULL)*/
    if (!empty(curr)) {
        
        /*There aren't negative positions on the list, so return with no actions done*/
        if(position < 0)
            return;

	/*Allocates memory for a new node to hold the card, and 
	if it's not capable, exits the main program*/
 	Card_node* new_node = create_card_node();
        if (empty(new_node)) {
            ERROR_MESSAGE();
        	exit(EXIT_FAILURE);
    	}
	
	/*Inserts on the first position of the list (head)*/
        if(position == 0) {
            new_node->card=data;
            new_node->next=*head;
            *head=new_node;
        }

        /*Search the node in the position specified, until the end of the list*/
        for(int i=1; i < position && curr!=NULL; i++) {
            curr=curr->next;
        }
	
	/*If the position doesn't exist (more than the size of the list) 
	returns with the list unchanged*/
        if(empty(curr)) {
            free(new_node);
            return;
        }

        /*Inserts the node in the specified 
        place with the specified data*/
        new_node->next=curr->next;
        new_node->card=data;
        curr->next=new_node;
    }
}

/* FUNCTION NAME:take_card_node
*  DESCRIPTION: Takes the node (if possible) from the selected 
*position of the list and returns a pointer to it*/
Card_node* take_card_node(Card_node** head, int position) {
    Card_node* tmp;
    Card_node* curr = *head;

	/*If the list isn't empty (not pointing to NULL)*/
	if(!empty(curr)) {
        	/*There aren't negative positions on the list, so return without no actions done*/
        	if(position < 0)
            		return NULL;
        	
        	/*Takes a node from the first position of the list (head)*/
        	else if(position == 0) {
            		*head = curr->next;
            		curr->next = NULL;
            		return curr;
        	}
		
		/*Search the node in the position specified, until the end of the list*/
        	for(int i=1; i < position && curr != NULL; i++)
        		curr=curr->next;
		
		/*If the position doesn't exist (more than the size of the list) 
		returns with the list unchanged*/
        	if(empty(curr)) {
            		return NULL;
        	}
        	
        	/*Takes node from the specified position*/
        	tmp = curr->next;
        	curr->next = curr->next->next;
        	tmp->next = NULL;

        	return tmp; //Returns a pointer to the taken node
    	}

    return NULL;
}

/* FUNCTION NAME:join_card_node
*  DESCRIPTION: Joins a node (if possible) in the selected,
*  else doesn't do anything*/
void join_card_node(Card_node** head, Card_node* jointo, int position) {
    Card_node* curr = *head;

	/*If the list isn't empty (not pointing to NULL)*/
	if(!empty(curr)) {
		/*There aren't negative positions on the list, so return without no actions done*/
		if(position < 0) {
			return;
		}
        	
        	/*Joins a node in the first position of the list (head)*/
        	if(position == 0){
			jointo->next = *head;
			*head = jointo;
			return;
		}
		
		/*Search the node in the position specified, until the end of the list*/
		for (int i=1; i < position && curr != NULL; i++)
			curr=curr->next;
		
		/*If the position doesn't exist (more than the size of the list) 
		returns with the list unchanged*/
		if(empty(curr)) {
			return;
		}
		
		/*Joins node to the list's specified position*/
		jointo->next = curr->next;
		curr->next = jointo;
	
	} else {//if the list is empty the node to join is in the head (independent of the position)
		*head = jointo;
	}
}

/* FUNCTION NAME:create_card_node
*  DESCRIPTION: Creates a node */
Card_node* create_card_node()
	
	/*Allocates memory for a new node and 
	if it's not capable, exits the main program*/
	Card_node* new_node = (Card_node *) malloc(sizeof(Card_node));
	if(empty(new_node)) {
        	ERROR_MESSAGE();
        	exit(EXIT_FAILURE);
    	}

	return new_node;//returns a pointer to the created node
}

/* FUNCTION NAME:erase_card_list
*  DESCRIPTION: Erases a list */
void erase_card_list(Card_node* head) {
    Card_node* tmp;
    
    /*Removes each list's element*/
    while((tmp = head) != NULL){
        head = head->next;
        free(tmp);
    }
}

///Stack

/* FUNCTION NAME:push_card
*  DESCRIPTION: Performs a push into the stack of a card */
void push_card(Card_node** head, Card data) {
       
       	/*Allocates memory for a new node to hold the card, and 
	if it's not capable, exits the main program*/
       Card_node* new_node = (Card_node *) malloc(sizeof(Card_node));
	if(empty(new_node)){
        	ERROR_MESSAGE();
        	exit(EXIT_FAILURE);
        }
	
	/*Push of the card into the stack*/
        new_node->card = data;
        new_node->next = *head;
        *head = new_node;
}

/* FUNCTION NAME:push_card_node
*  DESCRIPTION: Performs a push into the stack of a node */
void push_card_node(Card_node** head, Card_node* node){
	
	/*If there is a node (node pointer doesn't point to NULL)
	push the node into the stack*/
	if(!empty(node)){
		node->next = *head;
        *head = node;
	}
}

/* FUNCTION NAME:pop_card
*  DESCRIPTION: Performs a pop onto the stack */
Card_node *pop_card(Card_node** head) {
	Card_node* tmp = NULL;
	
	/*If the stack isn't empty (not pointing to NULL), pop*/
	if(!empty(*head)){
		tmp = *head;
		(*head) = tmp->next;
		tmp->next = NULL;
	}

	return tmp;//returns a pointer to the popped node
}

Card card_stack_top(Card_node* head) {
    return head->card;
}

int CountAces(Card_node *head) {
    Card_node *walk = head;
    int result = 0;
    
    if (walk != NULL) {
        while (walk != NULL) {
            if (IsAce(walk->card)) {
                result += 1;
            }
            
            walk = walk->next;
        }
    }
    
    return result;
}
