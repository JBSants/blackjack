#include "ai.h"

#include <stdlib.h>
#include "blackjack.h"

/**
 * GetCurrentAIAction: Returns ai action based on the current player points and house card
 * \param currentPlayer
 * \param house
 * \param ai_actions 2d matrix containing ai actions
 */
AIAction GetCurrentAIAction(Player_node *currentPlayer, Player *house, AIAction **ai_actions) {
    int numberOfAces = 0; // number of aces on the current player hand
    int aiColumn = 0; // selected column of ai matrix
    int aiRow = 0; // selected row of ai matrix
    
    if (currentPlayer != NULL && house != NULL && ai_actions != NULL) {
        numberOfAces = CountAces(currentPlayer->player.cards);
        
        /* Checks house shown card and selects column */
        if (house->cards->card.id > 9) {
            aiColumn = 8;
        } else {
            aiColumn = house->cards->card.id - 1;
        }
        
        /* Checks soft hand */
        if (numberOfAces > 0) {
            /* Checks player score and selects row */
            if (currentPlayer->player.score >= 19) {
                aiRow = 16;
            } else if (currentPlayer->player.score >= 13) {
                aiRow = currentPlayer->player.score - 3;
            }
        } else {
            /* Checks player score and selects row */
            if (currentPlayer->player.score >= 17) {
                aiRow = 9;
            } else if (currentPlayer->player.score > 8) {
                aiRow = currentPlayer->player.score - 8;
            }
        }
    }
    
    return ai_actions[aiRow][aiColumn];
}

/**
 * PlayAI: Plays ai, until it stands
 * \param currentPlayer
 * \param house
 * \param ai_actions 2d matrix containing ai actions
 * \param deck_head
 * \param numberOfDecks
 * \param hilo current hilo value
 * \param cardsDealt number of dealt cards
 */
void PlayAI(Player_node **currentPlayer, Player *house, AIAction **ai_actions, Card_node **deck_head, int numberOfDecks, int *hilo, int *cardsDealt) {
    Player_node *initialPlayer = NULL; // used to check if player has changed
    
    /* Error checking */
    if ((*currentPlayer) != NULL && (*currentPlayer)->player.ai) {
        initialPlayer = *currentPlayer;
        
        /* Gets ai action and executes it */
        switch (GetCurrentAIAction(*currentPlayer, house, ai_actions)) {
            case ActionHit:
                *currentPlayer = Hit(deck_head, *currentPlayer, numberOfDecks, hilo, cardsDealt);
                break;
            case ActionStand:
                *currentPlayer = Stand(*currentPlayer);
                break;
            case ActionDoubleOrHit:
                if ((*currentPlayer)->player.money >= (*currentPlayer)->player.bet && (*currentPlayer)->player.hand_size == BLACKJACK_INITIAL_CARDS) {
                    *currentPlayer = Double(deck_head, numberOfDecks, house, *currentPlayer, hilo, cardsDealt);
                } else {
                    *currentPlayer = Hit(deck_head, *currentPlayer, numberOfDecks, hilo, cardsDealt);
                }
                break;
            case ActionDoubleOrStand:
                if ((*currentPlayer)->player.money >= (*currentPlayer)->player.bet && (*currentPlayer)->player.hand_size == BLACKJACK_INITIAL_CARDS) {
                    *currentPlayer = Double(deck_head, numberOfDecks, house, *currentPlayer, hilo, cardsDealt);
                } else {
                    *currentPlayer = Stand(*currentPlayer);
                }
                break;
            case ActionSurrenderOrHit:
                if ((*currentPlayer)->player.hand_size == BLACKJACK_INITIAL_CARDS && (*currentPlayer)->player.hand_size == BLACKJACK_INITIAL_CARDS) {
                    *currentPlayer = Surrender(*currentPlayer);
                } else {
                    *currentPlayer = Hit(deck_head, *currentPlayer, numberOfDecks, hilo, cardsDealt);
                }
                break;
        }
        
        if (*currentPlayer == initialPlayer) {
            PlayAI(currentPlayer, house, ai_actions, deck_head, numberOfDecks, hilo, cardsDealt);
        }
    }
    
}


/* Destroys ai actions matrix */
void FreeAIActions(AIAction **ai_actions) {
    for (int i = 0; i < AIACTIONS_ROWS; i++) {
        free(ai_actions[i]);
    }
    
    free(ai_actions);
}
