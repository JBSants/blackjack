#include "deck.h"
#include "cards.h"

#include <stdlib.h>
#include <stdio.h>

Card_node* DeckMaker(int decks) {
	Card_node *head = NULL;
	Card_node *tail = head;
    
	for (int i = 0; i < decks * MAX_DECK_SIZE; i++) {
        
        Card_node *tmp = create_card_node();
        tmp->card.id = (i % MAX_SUIT_CARDS) + 1;
        tmp->card.suit = (i / MAX_SUIT_CARDS) % MAX_SUITS;
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

void ShuffleCards(Card_node** deck_head, int decks) {
	Card_node *card_taken = NULL;
	int deck_size = decks * MAX_DECK_SIZE;

    for(int i = 0; i < deck_size; i++) {
        int random = rand() % deck_size;
        card_taken = take_card_node(deck_head, random);
        join_card_node(deck_head, card_taken, i);
    }
}

void EraseDeck(Card_node* deck_head) {
    erase_card_list(deck_head);
}

int GetHiloScore(Card card) {
    if (card.id > 8) {
        return -1;
    }
    
    if (card.id > 5) {
        return 0;
    }
    
    return 1;
}

Card_node *NextCard(Card_node **deck_head, int numberOfDecks, int *hilo, int *cardsDealt) {
    Card_node *result = NULL;
    
    if(empty(*deck_head)) {
        *deck_head = DeckMaker(numberOfDecks);
        ShuffleCards(deck_head, numberOfDecks);
        
        *cardsDealt = 0;
        *hilo = 0;
    }
    
    result = take_card_node(deck_head, 0);
    
    *hilo += GetHiloScore(result->card);
    *cardsDealt += 1;
    
    return result;
}
