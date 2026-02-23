#ifndef MENU_H
#define MENU_H

#include "endgame.h"
#include "player.h"

void updateMenu(App *app, Player *player);
void renderMenu(App *app);

void updatePauseMenu(App *app, Level *level, Player *player);
void renderPauseMenu(App *app);

void updateGameOver(App *app, Level *level,Player *player, int CurrentLevel, SDL_Renderer *renderer);
void renderGameOver(App *app);
void update_settings(App *app);
void render_settings(App *app);
void draw_text(App *app, const char *msg, int x, int y);

#endif
