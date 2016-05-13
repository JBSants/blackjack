#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cards.h"
#include "blackjack.h"

void GetBankroll_GameResults(Player* house, Player_node* head, int bet) {
    while(head != NULL) {

        if(head->player.surrender==true)
        	head->player.surrender==false;
        else if (head->player.score > 21)
            head->player.games_result.lost ++;
        else if (house->score > 21) {
            head->player.money += 2 * bet + 0.5 * bet * (head->player.score == 21 && head->player.hand_size == 2);
            head->player.games_result.won += 1;
        }
        else if (house->score == head->player.score) {

            if (house->score == 21) {

                if (head->player.hand_size == house->size || (head->player.hand_size != 2 && house->hand_size != 2)) {
                    head->player.money += bet;
                    head->player.games_result.tied+=1;
                } else if (head->player.hand_size == 2 && house->hand_size != 2) {
                    head->player.money += 2.5 * bet;
                    head->player.games_result.won += 1;
                } else {
                    head->player.games_result.lost += 1;
                }

            } else {
                head->player.money += bet;
                head->player.games_result.tied += 1;
            }

        } else if (head->player.score != 21 || (head->player.score == 21 && head->player.hand_size != 2)) {
            head->player.money += 2 * bet * (head->player.score > house->score);

            if (head->player.score > house->score) {
                head->player.games_result.won +=1;
            } else {
                head->player.games_result.lost +=1;
            }

        } else {
            head->player.money += 2.5 * bet;
            head->player.games_result.won += 1;
        }

        head = head->next;
    }
}

Player_node* Hit(Card_node** deck_head, Player_node* current_player) {
    push_card_node(current_player->cards, NextCard(deck_head));
    current_player->hand_size += 1;

	GetScore(current_player);

	if (Bust(*current_player)) {
		return current_player->next;
	}

	return current_player;
}

Player_node* Stand(Player* house, Player_node* current_player) {
	return current_player->next;
}

Player_node* Double(Card_node **deckhead, int decks, Player *house, Player_node *current_player) {
	(current_player->player).money -= (current_player->player).bet;
	(current_player->player).bet *= 2;
	DealCards(deckhead, current_player, house, decks, 1, 1);

	return current_player->next;
}

Player_node* Surrender(Player_node *current_player) {
	current_player->player.surrender = true;
	current_player->player.money += 0.5 * (current_player->player.bet);
	current_player->player.games_result.lost += 1;
	return current_player->next;
}

void Bet(Player_node *head) {
	Player_node *tmp;
	char buf[8], buf2[8];
	float bet;
	short id=0;
    	int err = 0;

	do {
		tmp = head;
		printf("Name of the player: ");
		if (fgets(buf, sizeof(buf), stdin) != NULL) {
			id = 0;

			for(int i=0; i < 8; i++)
				id += tmp->player.name[i] == buf[i];

			do {
				if(id == 8) {
					do {
						printf("New bet value: ");

						fgets(buf2, sizeof(buf2), stdin);
						err = sscanf(buf2, "%f", &bet);

						if(err != 1 || bet < 2 || bet > head->player.money)
							printf("Bet value not valid!\n");
					} while(err != 1 || bet < 2 || bet > head->player.money);

					break;
				} else if((tmp = tmp->next) == NULL) {
					printf("There's no player with such name.\n");
                }
			} while(tmp != NULL);
		} else {
			printf("Not valid!\n");
        	}
	} while(id != 8);

	tmp->player.bet = bet;

}

int DealCards(Card_node** deck_head, Player_node* head, Player* house, int numberOfDecks, int numberOfCardsToDeal) {
    int cardsDealt = 0;

    if(house != NULL) {
        Player_node *walk = head;

		for(int i=0; i < numberOfCardsToDeal; i++) {

			while (walk != NULL) {
				push_card_node(walk->player.cards, NextCard(deck_head));
				walk->player.hand_size += 1;
                cardsDealt += 1;

				walk = walk->next;
			}

			push_existing_card(house->cards, NextCard(deck_head));
			house->hand_size++;
            cardsDealt += 1;
		}
	}

    return cardsDealt;
}

Card_node *NextCard(Card_node **deck_head) {
    if(empty(*deck_head)){
        *deck_head = DeckMaker(numberOfDecks);
        ShuffleCards(deck_head, numberOfDecks, 1); //TODO: Times really needed??
    }

    return take_card_node(deck_head, 0);
}
