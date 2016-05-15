#include "deck.h"
#include "cards.h"

#include <stdlib.h>
#include <stdio.h>

Card_node* DeckMaker(int decks) {
	Card_node* head = malloc(decks * 52 * sizeof(Card_node));
	Card_node* tmp = head;
	if(empty(head)){
		ERROR_MESSAGE;
		exit(EXIT_SUCCESS);
	}
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
			card_taken = take_card_node(deck_head, random);
			join_card_node(deck_head, card_taken, i);
		}
		times--;
	}
}

void EraseDeck(Card_node* deck_head) {
	Card_node *aux = NULL;

	while (deck_head != NULL) {
		aux = deck_head;
		deck_head = deck_head->next;

		free(aux);
	}
}
