#include "endgame.h"
#include "menu.h"

void toggleSound(Game *game) {
    if (game->volume > 0)
        game->volume = 0;
    else
        game->volume = 64;
    Mix_VolumeMusic(game->volume);
}

int isClicked(int mx, int my, SDL_Rect r) {
    return (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h);
}

void updateSettings(Game *game) {
    int mx, my;
    SDL_Rect backBtn = { 540, 500, 200, 60 };

    if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (isClicked(mx, my, backBtn)) {
            game->state = previousState;
            game->mouseReleased = false;
        }
    }
}

void renderSettings(Game *game) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    SDL_Rect backBtn = { 540, 500, 200, 60 };
    bool hovered = isClicked(mx, my, backBtn);

    if (game->settingsBg) {
        SDL_RenderCopy(game->renderer, game->settingsBg, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
    }

    if (game->backButton) {
        SDL_SetTextureColorMod(game->backButton, hovered ? 255 : 200, hovered ? 255 : 200, hovered ? 255 : 200);
        SDL_RenderCopy(game->renderer, game->backButton, NULL, &backBtn);
    } else {
        SDL_SetRenderDrawColor(game->renderer, 150 + (hovered ? 50 : 0), 150, 150, 255);
        SDL_RenderFillRect(game->renderer, &backBtn);
    }

    drawText(game, "SETTINGS", 540, 90);
}
