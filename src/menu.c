#include <SDL2/SDL.h>
#include "endgame.h"
#include "menu.h"
#include "player.h"

// Допоміжна функція: перевіряє, чи мишка знаходиться всередині прямокутника
bool isMouseInside(int mouseX, int mouseY, SDL_Rect rect) {
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

// --- ГОЛОВНЕ МЕНЮ ---
void updateMenu(App *app) {
    int mx, my;
    Uint32 mouseState = SDL_GetMouseState(&mx, &my);
    bool isLeftClicked = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

    // Розміри і позиції наших кнопок (по центру екрану)
    SDL_Rect btnPlay     = { 1280/2 - 100, 250, 200, 60 };
    SDL_Rect btnSettings = { 1280/2 - 100, 350, 200, 60 };
    SDL_Rect btnExit     = { 1280/2 - 100, 450, 200, 60 };

    // Якщо клікнули лівою кнопкою
    if (isLeftClicked) {
        if (isMouseInside(mx, my, btnPlay)) {
            Mix_HaltMusic();
            app->state = STATE_PLAY; // Запускаємо гру!

            // --- СКИДАЄМО СТАТИСТИКУ ПРИ НОВІЙ ГРІ ---
            app->deathCount = 0; 
            app->levelStartTime = SDL_GetTicks();
        }
        else if (isMouseInside(mx, my, btnSettings)) {
            app->state = STATE_SETTINGS; // Переходимо в налаштування
        }
        else if (isMouseInside(mx, my, btnExit)) {
            app->isRunning = false; // Виходимо з гри
        }
    }
}

void renderMenu(App *app) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    SDL_Rect btnPlay     = { 1280/2 - 100, 250, 200, 60 };
    SDL_Rect btnSettings = { 1280/2 - 100, 350, 200, 60 };
    SDL_Rect btnExit     = { 1280/2 - 100, 450, 200, 60 };

    // Малюємо кнопку PLAY (Зелена). Якщо мишка на ній - робимо яскравішою
    if (isMouseInside(mx, my, btnPlay)) SDL_SetRenderDrawColor(app->renderer, 100, 255, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 50, 200, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnPlay);

    // Малюємо кнопку SETTINGS (Жовта)
    if (isMouseInside(mx, my, btnSettings)) SDL_SetRenderDrawColor(app->renderer, 255, 255, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 200, 200, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnSettings);

    // Малюємо кнопку EXIT (Червона)
    if (isMouseInside(mx, my, btnExit)) SDL_SetRenderDrawColor(app->renderer, 255, 100, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 200, 50, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnExit);
}

// --- ЕКРАН GAME OVER ---
void updateGameOver(App *app, Level *level,Player *player) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    // Чекаємо натискання ENTER (Return) для рестарту
    if (keys[SDL_SCANCODE_RETURN]) {
        initPlayer(player, level->spawnX, level->spawnY);
        app->state = STATE_PLAY;

        // --- РЕСТАРТ ТАЙМЕРА РІВНЯ ---
        app->levelStartTime = SDL_GetTicks();
    }
    // Або ESC, щоб вийти в головне меню
    if (keys[SDL_SCANCODE_ESCAPE]) {
        initPlayer(player, level->spawnX, level->spawnY);
        app->state = STATE_MENU;
        Mix_PlayMusic(app->menuMusic, -1);
    }
}

void renderGameOver(App *app) {
    // Екран смерті буде темно-червоним
    SDL_SetRenderDrawColor(app->renderer, 100, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    SDL_Rect deadRect = { 1280/2 - 150, 768/2 - 50, 300, 100 };
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(app->renderer, &deadRect);
}