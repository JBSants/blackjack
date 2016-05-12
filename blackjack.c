#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cards.h"
#include "blackjack.h"

void GetBankroll_GameResults(Player* house, Node_player* head, int bet) {
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

Node_player* Hit(Card_node** deck_head, Player* house, int decks, Node_player* current_player, int option) {
	DealCards(deck_head, current_player, house, decks, 1, option);
	GetScore(&(current_player->player));

	if (current_player->player.score > 21){
		//Bust
		return current_player->next;
	}
	return current_player;
}

Node_player* Stand(Player* house, int decks, Node_player* current_player, int option) {
	if ((option % 5) != 0) {
		return current_player->next;
	} else {
		return NULL;
    }
}

Node_player* Double(Card_node **deckhead, int decks, Player *house, Node_player *current_player) {
	(current_player->player).money -= (current_player->player).bet;
	(current_player->player).bet *= 2;
	DealCards(deckhead, current_player, house, decks, 1, 1);

	return current_player->next;
}

Node_player* Surrender(Node_player *current_player) {
	current_player->player.surrender = true;
	current_player->player.money += 0.5 * (current_player->player.bet);
	current_player->player.games_result.lost += 1;
	return current_player->next;
}

void Bet(Node_player *head) {
	Node_player *tmp;
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
