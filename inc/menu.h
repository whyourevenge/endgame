#ifndef MENU_H
#define MENU_H

#include "endgame.h"
#include "player.h"
#include "level.h"

void updateMenu(Game *game, Player *player);
void renderMenu(Game *game);

void updatePauseMenu(Game *game, Level *level, Player *player);
void renderPauseMenu(Game *game);

void updateGameOver(Game *game, Level *level, Player *player, int currentLevel, SDL_Renderer *renderer);
void renderGameOver(Game *game);

void updateSettings(Game *game);
void renderSettings(Game *game);
void toggleSound(Game *game);

void drawText(Game *game, const char *msg, int x, int y);

#endif
