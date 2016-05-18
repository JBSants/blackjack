#ifndef AI_H
#define AI_H

#include "players.h"
#include "cards.h"

#define AIACTIONS_COLUMNS 10
#define AIACTIONS_ROWS 17
#define AI "EA"
#define NOT_AI "HU"

typedef enum { ActionHit = 0, ActionStand = 1, ActionDoubleOrHit = 2, ActionDoubleOrStand = 3, ActionSurrenderOrHit = 4 } AIAction;

void PlayAI(Player_node **currentPlayer, Player *house, AIAction **ai_actions, Card_node **deck_head, int numberOfDecks, int *hilo, int *cardsDealt);
void FreeAIActions(AIAction **ai_actions);

#endif
