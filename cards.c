#include "cards.h"
#include <stdio.h>
#include <stdlib.h>

stat add_card(Card_node** head, Card data) {

	if(empty(*head))
		return NOP_STAT;

	Card_node* new_node = create_node_card(data);
	if(new_node == NULL) {
        	ERROR_MESSAGE;
        	exit(EXIT_FAILURE);
    	}

	if(new_node == NULL)
		return NOP_STAT;

	new_node->next = *head;
	new_node->card = data;
	*head = new_node;

	return ACT_STAT;
}

stat insert_card(Card_node** head, Card data, int position) {
    Card_node* curr = *head;

    if(!empty(curr)) {
        if(position < 0)
            return NOP_STAT;

        Card_node* new_node = create_node_card(data);
        if(empty(new_node)) {
		ERROR_MESSAGE;
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
            return NOP_STAT;
        }

        new_node->next=curr->next;
        new_node->card=data;
        curr->next=new_node;

        return ACT_STAT;
    }

    return EMPTY_STAT;
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

stat join_card_node(Card_node** head, Card_node* jointo, int position) {
    Card_node* curr = *head;

	if(!empty(curr)) {

		if(position < 0) {
			return NOP_STAT;
		} else if(position == 0){
			jointo->next = *head;
			*head = jointo;
			return ACT_STAT;
		}

		for(int i=1; i < position && curr != NULL; i++)
			curr=curr->next;

		if(empty(curr)) {
			return NOP_STAT;
		}

		jointo->next = curr->next;
		curr->next = jointo;
	} else {
		*head = jointo;
		return ACT_STAT;
	}

	return WARN_STAT;
}

Card_node* create_card_node(Card data) {
    Card_node* new_node = malloc(sizeof(Card_node));

    if(empty(new_node)) {
        ERROR_MESSAGE;
        exit(EXIT_FAILURE);
    }

    return new_node;
}

void erase_card_list(Card_node* head) {
    Card_node* tmp;
    while((tmp=head) != NULL){
        head = head->next;
        free(tmp);
    }
}

///Stack

void push_card(Card_node** head, Card data) {
        Card_node* new_node = malloc(sizeof(Card_node));

        if(empty(new_node)){
        	ERROR_MESSAGE;
        	exit(EXIT_FAILURE);
        }

        new_node->card = data;
        new_node->prev = *head;
        *head = new_node;
}

void push_card_node(Card_node** head, Card_node* node){
	if(!empty(node)){
		node->prev = *head;
        *head = node;
	}
}


Card pop_card(Card_node** head) {
	Card_node* tmp;
	Card data;

	data.id = -1;
	data.suit = 0;

	if(!empty(*head)){
		data = (*head)->card;
		tmp = *head;
		(*head) = (*head)->prev;
		free(tmp);
		return data;
	}

	return data;
}

Card card_stack_top(Card_node* head) {
    return head->card;
}

void card_stack_erase(Card_node** head) {
	Card_node* tmp;

	while((tmp=*head)!=NULL){
		*head = (*head)->prev;
		free(tmp);
    }
}
