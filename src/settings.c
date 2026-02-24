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
    Uint32 mouseState = SDL_GetMouseState(&mx, &my);
    bool isLeftDown = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
    SDL_Rect backBtn = { 540, 500, 200, 60 };
    SDL_Rect minusBtn = { 400, 300, 80, 80 };
    SDL_Rect plusBtn = { 800, 300, 80, 80 };

    if (!isLeftDown)
        app->mouseReleased = true;

    if (isLeftDown) {
        if (isClicked(mx, my, backBtn) && app->mouseReleased) {
            app->state = previousState;
            app->mouseReleased = false;
        }

        if (isClicked(mx, my, plusBtn) && app->mouseReleased) {
            if (app->volume >= 118) 
                app->volume = 128;
            else 
                app->volume += 10;
            Mix_VolumeMusic(app->volume);
            Mix_Volume(-1, app->volume);
            app->mouseReleased = false;
        }

        if (isClicked(mx, my, minusBtn) && app->mouseReleased) {
            if (app->volume <= 10) 
                app->volume = 0;
            else 
                app->volume -= 10;
            Mix_VolumeMusic(app->volume);
            Mix_Volume(-1, app->volume);
            app->mouseReleased = false;
        }
    }
}

void renderSettings(App *app) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    SDL_Rect backBtn = { 540, 500, 200, 60 };
    SDL_Rect minusBtn = { 400, 300, 80, 80 };
    SDL_Rect plusBtn = { 800, 300, 80, 80 };
    bool hovered = isClicked(mx, my, backBtn);

    if (app->settingsBg) {
        SDL_RenderCopy(app->renderer, app->settingsBg, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
    }

    if (app->minusButton) {
        if (isClicked(mx, my, minusBtn))
            SDL_SetTextureColorMod(app->minusButton, 255, 255, 255);
        else
            SDL_SetTextureColorMod(app->minusButton, 200, 200, 200);
        SDL_RenderCopy(app->renderer, app->minusButton, NULL, &minusBtn);
    }

    if (app->plusButton) {
        if (isClicked(mx, my, plusBtn))
            SDL_SetTextureColorMod(app->plusButton, 255, 255, 255);
        else
            SDL_SetTextureColorMod(app->plusButton, 200, 200, 200);
        SDL_RenderCopy(app->renderer, app->plusButton, NULL, &plusBtn);
    }

    if (app->backButton) {
        SDL_SetTextureColorMod(app->backButton, hovered ? 255 : 200, hovered ? 255 : 200, hovered ? 255 : 200);
        SDL_RenderCopy(app->renderer, app->backButton, NULL, &backBtn);
    } else {
        SDL_SetRenderDrawColor(app->renderer, 150 + (hovered ? 50 : 0), 150, 150, 255);
        SDL_RenderFillRect(app->renderer, &backBtn);
    }

    drawText(app, "SETTINGS", 540, 90);
    
    char volText[32];
    sprintf(volText, "VOLUME: %d", app->volume);
    drawText(app, volText, 535, 310);
}
