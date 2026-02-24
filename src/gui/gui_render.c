#include "../../inc/gui.h"

void GUI_DrawText(Game *game, const char *msg, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surf = TTF_RenderUTF8_Blended(game->font, msg, color);
    if (!surf) return;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(game->renderer, surf);
    SDL_Rect dst = {x, y, surf->w, surf->h};
    
    SDL_RenderCopy(game->renderer, tex, NULL, &dst);
    
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void GUI_DrawButton(Game *game, SDL_Texture *texture, SDL_Rect rect, int mx, int my) {
    bool hovered = GUI_IsMouseInside(mx, my, rect);

    if (texture != NULL) {
        Uint8 colorMod = hovered ? 255 : 200;
        SDL_SetTextureColorMod(texture, colorMod, colorMod, colorMod);
        SDL_RenderCopy(game->renderer, texture, NULL, &rect);
    } else {
        if (hovered) {
            SDL_SetRenderDrawColor(game->renderer, 100, 255, 100, 255);
        } else {
            SDL_SetRenderDrawColor(game->renderer, 50, 200, 50, 255);
        }
        SDL_RenderFillRect(game->renderer, &rect);
    }
}