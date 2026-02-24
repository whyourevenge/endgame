#include "endgame.h"
#include "menu.h"

int is_clicked(int mx, int my, SDL_Rect r) {
    return (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h);
}

void update_settings(App *app) {
    int mx, my;
    SDL_Rect back_btn = { 540, 500, 200, 60 };

    if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (is_clicked(mx, my, back_btn)) {
            app->state = STATE_MENU;
            app->mouseReleased = false;
        }
    }
}

void render_settings(App *app) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    SDL_Rect back_btn = { 540, 500, 200, 60 };
    bool hovered = is_clicked(mx, my, back_btn);

    if (app->settingsBg) {
        SDL_RenderCopy(app->renderer, app->settingsBg, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
    }

    if (app->backButton) {
        SDL_SetTextureColorMod(app->backButton, hovered ? 255 : 200, hovered ? 255 : 200, hovered ? 255 : 200);
        SDL_RenderCopy(app->renderer, app->backButton, NULL, &back_btn);
    } else {
        SDL_SetRenderDrawColor(app->renderer, 150 + (hovered ? 50 : 0), 150, 150, 255);
        SDL_RenderFillRect(app->renderer, &back_btn);
    }

    draw_text(app, "SETTINGS", 540, 90);
}
