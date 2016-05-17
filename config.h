#ifndef CONFIG_H
#define CONFIG_H

#include "players.h"
#include "ai.h"

void GameSettings(char *config_file, char *ai, int *decks, Player_node **resultPlayers, AIAction ***);
void AddNewPlayer(Player_node **players, int position);

#endif