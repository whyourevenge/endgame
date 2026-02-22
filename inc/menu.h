#ifndef MENU_H
#define MENU_H

#include "endgame.h"
#include "player.h"

void updateMenu(App *app);
void renderMenu(App *app);

void updatePauseMenu(App *app);
void renderPauseMenu(App *app);

void updateGameOver(App *app, Level *level,Player *player);
void renderGameOver(App *app);

#endif