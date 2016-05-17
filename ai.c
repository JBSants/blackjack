#include "ai.h"

#include <stdlib.h>
#include "blackjack.h"

AIAction GetCurrentAIAction(Player_node *currentPlayer, Player *house, AIAction **ai_actions) {
    int numberOfAces = CountAces(currentPlayer->player.cards);
    int aiColumn = 0;
    int aiRow = 0;
    
    if (house->cards->card.id > 9) {
        aiColumn = 8;
    } else {
        aiColumn = house->cards->card.id - 1;
    }
    
    if (numberOfAces > 0) {
        if (currentPlayer->player.score >= 19) {
            aiRow = 16;
        } else if (currentPlayer->player.score >= 13) {
            aiRow = currentPlayer->player.score - 3;
        }
    } else {
        if (currentPlayer->player.score >= 17) {
            aiRow = 9;
        } else if (currentPlayer->player.score > 8) {
            aiRow = currentPlayer->player.score - 8;
        }
    }
    
    return ai_actions[aiRow][aiColumn];
}

void PlayAI(Player_node **currentPlayer, Player *house, AIAction **ai_actions, Card_node **deck_head, int numberOfDecks) {
    if (currentPlayer != NULL) {
        switch (GetCurrentAIAction(*currentPlayer, house, ai_actions)) {
            case ActionHit:
                *currentPlayer = Hit(deck_head, *currentPlayer, numberOfDecks);
                break;
            case ActionStand:
                *currentPlayer = Stand(*currentPlayer);
                break;
            case ActionDoubleOrHit:
                if ((*currentPlayer)->player.money >= (*currentPlayer)->player.bet && (*currentPlayer)->player.hand_size == BLACKJACK_INITIAL_CARDS) {
                    *currentPlayer = Double(deck_head, numberOfDecks, house, *currentPlayer);
                } else {
                    *currentPlayer = Hit(deck_head, *currentPlayer, numberOfDecks);
                }
                break;
            case ActionDoubleOrStand:
                if ((*currentPlayer)->player.money >= (*currentPlayer)->player.bet) {
                    *currentPlayer = Double(deck_head, numberOfDecks, house, *currentPlayer);
                } else {
                    *currentPlayer = Stand(*currentPlayer);
                }
                break;
            case ActionSurrenderOrHit:
                if ((*currentPlayer)->player.hand_size == BLACKJACK_INITIAL_CARDS && (*currentPlayer)->player.hand_size == BLACKJACK_INITIAL_CARDS) {
                    *currentPlayer = Surrender(*currentPlayer);
                } else {
                    *currentPlayer = Hit(deck_head, *currentPlayer, numberOfDecks);
                }
                break;
        }
        
        if (*currentPlayer != NULL && (*currentPlayer)->player.ai) {
            PlayAI(currentPlayer, house, ai_actions, deck_head, numberOfDecks);
        }
    }
    
}

void FreeAIActions(AIAction **ai_actions) {
    for (int i = 0; i < AIACTIONS_ROWS; i++) {
        free(ai_actions[i]);
    }
    
    free(ai_actions);
}