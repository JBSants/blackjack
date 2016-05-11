#include "cards.h"
#include <stdio.h>
#include <stdlib.h>

stat add_card(Card_node** head, Card data) {

	if(card_list_empty(*head))
		return NOP_STAT;

	Card_node* new_node = create_node_card(data);

	if(new_node == NULL)
		return NOP_STAT;

	new_node->next = *head;
	new_node->card = data;
	*head = new_node;

	return ACT_STAT;
}

stat insert_card(Card_node** head,Card data, int position) {
    Card_node* curr = *head;

    if(!card_list_empty(curr)) {
        if(position < 0)
            return NOP_STAT;

        Card_node* new_node = create_node_card(data);

        if(position == 0) {
            new_node->card=data;
            new_node->next=*head;
            *head=new_node;
        }

        for(int i=1; i < position && curr!=NULL; i++) {
            curr=curr->next;
        }

        if(curr == NULL) {
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

Card_node* take_node_card(Card_node** head, int position) {
    Card_node* tmp;
    Card_node* curr = *head;

    if(!card_list_empty(curr)) {
        if(position < 0)
            return NULL;
        else if(position == 0) {
            *head = curr->next;
            curr->next = NULL;
            return curr;
        }

        for(int i=1; i < position && curr != NULL; i++)
            curr=curr->next;

        if(curr == NULL) {
            return NULL;
        }

        tmp = curr->next;
        curr->next = curr->next->next;
        tmp->next = NULL;

        return tmp;
    }

    return NULL;
}

stat join_node_card(Card_node** head, Card_node* jointo, int position) {
    Card_node* curr = *head;

	if(!card_list_empty(curr)) {

		if(position < 0) {
			return NOP_STAT;
		} else if(position == 0){
			jointo->next = *head;
			*head = jointo;
			return ACT_STAT;
		}

		for(int i=1; i < position && curr != NULL; i++)
			curr=curr->next;

		if(curr == NULL) {
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

bool card_list_empty(Card_node* head){
	return head == NULL;
}

Card_node* create_node_card(Card data) {
    Card_node* new_node = malloc(sizeof(Card_node));

    if(new_node == NULL) {
        printf("Memory allocation failed! Bye.");
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

void push_card(CardStack* stack, Card data) {
    if(stack->size > 0) {
        CardStack_node* old_node = malloc(sizeof(CardStack_node));
        old_node->card = stack->top->card;
        old_node->prev = stack->top->prev;
        stack->top->prev = old_node->prev;
    }

    stack->top->card = data;
    stack->size++;
}

Card pop_card(CardStack* stack) {
	CardStack_node* tmp;

	Card data = stack->top->card;
	stack->top->card = stack->top->prev->card;
	tmp = stack->top->prev;
	stack->top->prev = stack->top->prev->prev;
	stack->size--;

	free(tmp);

	return data;
}

Card card_stack_top(CardStack* stack) {
    return stack->top->card;
}

int card_stack_size(CardStack* stack) {
    return stack->size;
}

void card_stack_erase(CardStack* stack) {
    CardStack_node* curr, *tmp;
    int stack_size = card_stack_size(stack);


    Card* data = malloc((stack_size) * sizeof(Card));
    curr = stack->top->prev;
    data[stack_size - 1] = stack->top->card;
    stack_size -= 2;
    free(stack);

    for(; 0 <= stack_size; stack_size--){
        tmp = curr;
        data[stack_size] = curr->card;
        curr = curr->prev;

        free(tmp);
    }
}
