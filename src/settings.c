#include "endgame.h"
#include "menu.h"

void toggleSound(App *app) {
    if (app->volume > 0)
        app->volume = 0;
    else
        app->volume = 64;
    Mix_VolumeMusic(app->volume);
}

int isClicked(int mx, int my, SDL_Rect r) {
    return (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h);
}

void updateSettings(App *app) {
    int mx, my;
    SDL_Rect backBtn = { 540, 500, 200, 60 };

    if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (isClicked(mx, my, backBtn)) {
            app->state = STATE_MENU;
            app->mouseReleased = false;
        }
    }
}

void renderSettings(App *app) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    SDL_Rect backBtn = { 540, 500, 200, 60 };
    bool hovered = isClicked(mx, my, backBtn);

    if (app->settingsBg) {
        SDL_RenderCopy(app->renderer, app->settingsBg, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
    }

    if (app->backButton) {
        SDL_SetTextureColorMod(app->backButton, hovered ? 255 : 200, hovered ? 255 : 200, hovered ? 255 : 200);
        SDL_RenderCopy(app->renderer, app->backButton, NULL, &backBtn);
    } else {
        SDL_SetRenderDrawColor(app->renderer, 150 + (hovered ? 50 : 0), 150, 150, 255);
        SDL_RenderFillRect(app->renderer, &backBtn);
    }

    drawText(app, "SETTINGS", 540, 90);
}
