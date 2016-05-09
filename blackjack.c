#include <stdlib.h>
#include "blackjack.h"

void GetBankroll_GameResults(Player* house, Node_player* head, int bet) {
    while(head != NULL) {

        if (head->player.score>21) {
            head->player.games_result.tied+=(house->score>21);
            head->player.games_result.lost+=(house->score<=21);
        } else if (house->score>21) {
            head->player.money += 2 * bet + 0.5 * bet * (head->player.score == 21 && head->player.cards->size == 2);
            head->player.games_result.won += 1;
        } else if (house->score == head->player.score) {

            if (house->score == 21) {

                if (head->player.cards->size == house->cards->size || (head->player.cards->size != 2 && house->cards->size != 2)) {
                    head->player.money += bet;
                    head->player.games_result.tied+=1;
                } else if (head->player.cards->size == 2 && house->cards->size != 2) {
                    head->player.money += 2.5 * bet;
                    head->player.games_result.won += 1;
                } else {
                    head->player.games_result.lost += 1;
                }

            } else {
                head->player.money += bet;
                head->player.games_result.tied += 1;
            }

        } else if (head->player.score != 21 || (head->player.score == 21 && head->player.cards->size != 2)) {
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
    /*****/
    /*TODO*/
    /******/

	//DealCards(deck_head, current_player, house, decks, 1, option);

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
