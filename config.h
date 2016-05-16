#ifndef CONFIG_H
#define CONFIG_H

#include "players.h"

void GameSettings(char *config_file, char *ai, int *decks, Player_node **resultPlayers);
void AddNewPlayer(Player_node **players, int position);

#endif