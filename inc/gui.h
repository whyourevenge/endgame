#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "endgame.h"

bool GUI_IsMouseInside(int mouseX, int mouseY, SDL_Rect rect);
bool GUI_isButtonClicked(SDL_Rect rect, int mx, int my, bool isLeftClicked, bool mouseReleased);

void GUI_DrawText(Game *game, const char *msg, int x, int y);
void GUI_DrawButton(Game *game, SDL_Texture *texture, SDL_Rect rect, int mx, int my);

#endif
