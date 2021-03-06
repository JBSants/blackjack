#ifndef CARDS_H
#define CARDS_H

#include <stdbool.h>

#define MAX_DECK_SIZE 52
#define ALLOCATION_ERROR_MESSAGE() printf("Memory allocation failed! Bye.")
#define empty(head) (head==NULL)
#define IsAce(card) card.id == 13
#define MAX_ACE_POINTS 11

typedef enum { Clubs = 0, Diamonds = 1, Hearts = 2, Spades = 3} Suit;

typedef struct {
    short id;
    Suit suit;
} Card;

typedef struct card_node {
	Card card;
	struct card_node* next;
} Card_node;

void add_card(Card_node** head, Card data);
void insert_card(Card_node** head,Card data, int position);
Card_node* take_card_node(Card_node** head, int position);
void join_card_node(Card_node** head, Card_node* jointo, int position);
Card_node* create_card_node();
void erase_card_list(Card_node* head);

void push_card(Card_node** head, Card data);
void push_card_node(Card_node** head, Card_node* node);
Card_node *pop_card(Card_node** head);

int CountAces(Card_node *head);

#endif
