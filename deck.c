#include "deck.h"
#include "cards.h"

#include <stdlib.h>
#include <stdio.h>

/* FUNCTION NAME:DeckMaker
*  DESCRIPTION: Creates a deck made of several decks with 52 different cards */
Card_node* DeckMaker(int decks) {
	Card_node *head = NULL;
	Card_node *tail = head;
    	
    	/* Untill the deck is of the specified size puts cards in order*/
	for (int i = 0; i < decks * MAX_DECK_SIZE; i++) {
        	/* Creates a node if possible, if not
		exits the main program */
        	Card_node *tmp = create_card_node();
        	
        	/* Puts up the card and sets the node next field to NULL*/
        	tmp->card.id = (i % MAX_SUIT_CARDS) + 1;
        	tmp->card.suit = (i / MAX_SUIT_CARDS) % MAX_SUITS;
        	tmp->next = NULL;
        	
        	/* If the list is empty, inserts the node in the the 
        	list with the head and tail pointing to it */
        	if (head == NULL) {
            		head = tmp;
            		tail = head;
        	/* Inserts the node in the end of the list */
        	} else {
            		tail->next = tmp;
            		tail = tmp;
        	}
	}

	return head; // Returns the head of the deck
}


/* FUNCTION NAME:ShuffleCards
*  DESCRIPTION: Shuffles the list's elements */
void ShuffleCards(Card_node** deck_head, int decks) {
	Card_node *card_taken = NULL;
	int deck_size = decks * MAX_DECK_SIZE;
	
	/* Walks through all elements in the list and exchange each element
	with a random one in the list */
	for(int i = 0; i < deck_size; i++) {
        	int random = rand() % deck_size;
        	card_taken = take_card_node(deck_head, random);
        	join_card_node(deck_head, card_taken, i);
    }
}

/* FUNCTION NAME:EraseDeck
*  DESCRIPTION: Erases the deck */
void EraseDeck(Card_node* deck_head) {
    erase_card_list(deck_head);
}

/* FUNCTION NAME:GetHiloScore
*  DESCRIPTION: Calculates a card Hilo corresponding score */
int GetHiloScore(Card card) {
    if (card.id > 8) {
        return -1;
    }
    
    if (card.id > 5) {
        return 0;
    }
    
    return 1;
}

/* FUNCTION NAME:NextCard
*  DESCRIPTION: Returns the next card bo be dealt, increments the number of cards dealt
and calculates the hilo score of the card and sums it with the accumulated*/
Card_node *NextCard(Card_node **deck_head, int numberOfDecks, int *hilo, int *cardsDealt) {
    Card_node *result = NULL;
    
    /* If the deck is empty makes a new one, shuffles it and resets
    the hilo score and the number of cards dealt */
    if(empty(*deck_head)) {
        *deck_head = DeckMaker(numberOfDecks);
        ShuffleCards(deck_head, numberOfDecks);
        
        *cardsDealt = 0;
        *hilo = 0;
    }
    
    result = take_card_node(deck_head, 0); // Takes a card (node) from the deck
    
    *hilo += GetHiloScore(result->card); // Calculates card's hilo score and sums it to the accumulated
    *cardsDealt += 1; // Increments the number of cards dealt
    
    return result; // Returns a pointer to the card taken from the deck
}
