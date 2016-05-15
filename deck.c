#include "deck.h"
#include "cards.h"

#include <stdlib.h>
#include <stdio.h>

Card_node* DeckMaker(int decks) {
	Card_node* head = NULL;
    
	Card_node* tail = head;
    
	for (int i=0; i < decks * 52; i++) {
        
        Card_node *tmp = create_card_node();
        tmp->card.id = (i % 13) + 1;
        tmp->card.suit = (i / 13) % 4;
        tmp->next = NULL;
        
        if (head == NULL) {
            head = tmp;
            tail = head;
        } else {
            tail->next = tmp;
            tail = tmp;
        }
	}

	return head;
}

void ShuffleCards(Card_node** deck_head, int decks,int times) {
	Card_node* card_taken;
	int deck_size = decks * MAX_DECK_SIZE;

	while(times > 0) {
		for(int i=0; i < deck_size; i++) {
			int random = rand() % deck_size;
			card_taken = take_card_node(deck_head, random);
			join_card_node(deck_head, card_taken, i);
		}
		times--;
	}
}

void EraseDeck(Card_node* deck_head) {
    card_stack_erase(&deck_head);
}
