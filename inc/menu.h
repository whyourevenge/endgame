#ifndef MENU_H
#define MENU_H

#include "endgame.h"
#include "player.h"
#include "level.h"

void updateMenu(App *app, Player *player);
void renderMenu(App *app);

void updatePauseMenu(App *app, Level *level, Player *player);
void renderPauseMenu(App *app);

void updateGameOver(App *app, Level *level, Player *player, int currentLevel, SDL_Renderer *renderer);
void renderGameOver(App *app);

void updateSettings(App *app);
void renderSettings(App *app);
void toggleSound(App *app);

void drawText(App *app, const char *msg, int x, int y);

#endif
