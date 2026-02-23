#include "endgame.h"
#include "menu.h"


int is_clicked(int mx, int my, SDL_Rect r) {
    return (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h);
}

void update_settings(App *app) {
    int mx;
    int my;
    SDL_Rect back_btn;

    back_btn = (SDL_Rect){540, 500, 200, 60};
    if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (is_clicked(mx, my, back_btn))
            app->state = STATE_MENU;
    }
}

void render_settings(App *app) {
    SDL_Rect back_btn;

    back_btn = (SDL_Rect){540, 500, 200, 60};
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);
    SDL_SetRenderDrawColor(app->renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(app->renderer, &back_btn);
    draw_text(app, "SETTINGS", 540, 100);
    draw_text(app, "BACK TO MENU", 550, 515);
}
