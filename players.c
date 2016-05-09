#include <stdlib.h>
#include "players.h"

void GetScore(Player* player) {
    const short card_points[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

    short score = 0, aces = 0;
    CardStack_node* cardnode_ptr;


    cardnode_ptr->card = player->cards->top->card;
    cardnode_ptr->prev = player->cards->top->prev;

    // calculate the score excluding the aces
    while(cardnode_ptr != NULL) {

        if (cardnode_ptr->card.id != 12) {
            score += card_points[cardnode_ptr->card.id];
        } else {
            aces++;
        }
        cardnode_ptr = cardnode_ptr->prev;
    }

    // calculates the aces' points and sums it to the rest of the score
    for(;aces>0;aces--) {
        if ((score + 11) <= 22 - aces) {
            score += 11;
        } else {
            score += 1;
        }
    }
    player->score = score;
}
