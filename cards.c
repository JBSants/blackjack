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

void insert_card(Card_node** head, Card data, int position) {
    Card_node* curr = *head;

    if (!empty(curr)) {
        if(position < 0)
            return;

        Card_node* new_node = create_card_node();
        
        if (empty(new_node)) {
            ERROR_MESSAGE();
        	exit(EXIT_FAILURE);
    	}

        if(position == 0) {
            new_node->card=data;
            new_node->next=*head;
            *head=new_node;
        }

        for(int i=1; i < position && curr!=NULL; i++) {
            curr=curr->next;
        }

        if(empty(curr)) {
            free(new_node);
            return;
        }

        new_node->next=curr->next;
        new_node->card=data;
        curr->next=new_node;
    }
}

Card_node* take_card_node(Card_node** head, int position) {
    Card_node* tmp;
    Card_node* curr = *head;

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

void join_card_node(Card_node** head, Card_node* jointo, int position) {
    Card_node* curr = *head;

	if(!empty(curr)) {

		if(position < 0) {
			return;
		}
        
        if(position == 0){
			jointo->next = *head;
			*head = jointo;
			return;
		}

		for (int i=1; i < position && curr != NULL; i++)
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

Card_node* create_card_node() {
    Card_node* new_node = (Card_node *) malloc(sizeof(Card_node));

    if(empty(new_node)) {
        ERROR_MESSAGE();
        exit(EXIT_FAILURE);
    }

    return new_node;
}

void erase_card_list(Card_node* head) {
    Card_node* tmp;
    
    while((tmp = head) != NULL){
        head = head->next;
        free(tmp);
    }
}

///Stack

void push_card(Card_node** head, Card data) {
        Card_node* new_node = (Card_node *) malloc(sizeof(Card_node));

        if(empty(new_node)){
        	ERROR_MESSAGE();
        	exit(EXIT_FAILURE);
        }

        new_node->card = data;
        new_node->next = *head;
        *head = new_node;
}

void push_card_node(Card_node** head, Card_node* node){
	if(!empty(node)){
		node->next = *head;
        *head = node;
	}
}


Card_node *pop_card(Card_node** head) {
	Card_node* tmp = NULL;

	if(!empty(*head)){
		tmp = *head;
		(*head) = tmp->next;
		tmp->next = NULL;
	}

	return tmp;
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
