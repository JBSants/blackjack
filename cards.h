#ifndef CARDS_H
#define CARDS_H

#include <stdbool.h>

#define MAX_DECK_SIZE 52

typedef enum { Diamonds, Hearts, Clubs, Spades } Suit;
typedef enum { NOP_STAT=-1, EMPTY_STAT, ACT_STAT, WARN_STAT } stat;

typedef struct {
    short id;
    Suit suit;
} Card;

typedef struct card_node {
	Card card;
	struct card_node* next;
} Card_node;

typedef struct cardstack_node{
	Card card;
	struct cardstack_node* prev;
} CardStack_node;

typedef struct cardstack {
	int size;
	CardStack_node* top;
} CardStack;


stat add_card(Card_node** head, Card data);
stat insert_card(Card_node** head,Card data, int position);
Card_node* take_node_card(Card_node** head, int position);
stat join_node_card(Card_node** head, Card_node* jointo, int position);
bool card_list_empty(Card_node* head);
Card_node* create_node_card(Card data);
void erase_card_list(Card_node* head);

void push_card(CardStack* stack, Card data);
Card pop_card(CardStack* stack);
Card card_stack_top(CardStack* stack);
int card_stack_size(CardStack* stack);
void card_stack_erase(CardStack* stack);

#endif
