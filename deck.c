#include "deck.h"

#include <stdlib.h>

Card_node* CreateDeck(int decks) {
	Card_node* head = malloc(decks * 52 * sizeof(Card_node));
	Card_node* tmp = head;
	if(card_list_empty(head))
		return head;

	for (int i=0; i < decks * 52; i++) {
		tmp->next = tmp + 1;
		tmp->card.id = i % 13;
		tmp->card.suit = i / 13 % 4;
		tmp = tmp->next;
	}

	(tmp - 1)->next = NULL;

	return head;
}

void ShuffleCards(Card_node** deck_head, int decks,int times) {
	Card_node* card_taken;
	int deck_size = decks * MAX_DECK_SIZE;

	while(times > 0) {
		for(int i=0; i < deck_size; i++) {
			int random = rand() % deck_size;
			card_taken = take_node_card(deck_head, random);
			join_node_card(deck_head, card_taken, i);
		}

		times--;
	}
}
