#include "endgame.h"

void drawText(Game *game, const char *msg, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surf;
    SDL_Texture *tex;
    SDL_Rect dst;

    surf = TTF_RenderUTF8_Blended(game->font, msg, color);
    tex = SDL_CreateTextureFromSurface(game->renderer, surf);
    dst = (SDL_Rect){x, y, surf->w, surf->h};
    SDL_RenderCopy(game->renderer, tex, NULL, &dst);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}
