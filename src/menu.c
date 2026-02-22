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

    if (!isLeftClicked) {
        app->mouseReleased = true;
    }

    // Розміри і позиції наших кнопок (по центру екрану)
    SDL_Rect btnPlay     = { 1280/2 - 100, 250, 200, 60 };
    SDL_Rect btnSettings = { 1280/2 - 100, 350, 200, 60 };
    SDL_Rect btnExit     = { 1280/2 - 100, 450, 200, 60 };

    // Якщо клікнули лівою кнопкою
    if (isLeftClicked && app->mouseReleased) {
        if (isMouseInside(mx, my, btnPlay)) {
            Mix_HaltMusic();
            app->state = STATE_PLAY; // Запускаємо гру!

            // --- СКИДАЄМО СТАТИСТИКУ ПРИ НОВІЙ ГРІ ---
            app->state = STATE_PLAY;
            app->currentLevel = 1;
            app->deathCount = 0;
            app->gameStartTime = SDL_GetTicks();
            
            if (app->level1Music) 
                Mix_PlayMusic(app->level1Music, -1);

        }
        
        else if (isMouseInside(mx, my, btnSettings)) {
            app->mouseReleased = false;
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

// --- МЕНЮ ПАУЗИ ---
void updatePauseMenu(App *app) {
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    bool isLeftClicked = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

    if (!isLeftClicked) {
        app->mouseReleased = true;
    }

    int btnW = 200;
    int btnH = 60;
    int centerX = (WINDOW_WIDTH - btnW) / 2;

    SDL_Rect btnPauseResume     = { centerX, 250, btnW, btnH};
    SDL_Rect btnPauseSettings = { centerX, 350, btnW, btnH };
    SDL_Rect btnPauseExit     = { centerX, 450, btnW, btnH };

    if (isLeftClicked && app->mouseReleased) {
        if (isMouseInside(mouseX, mouseY, btnPauseResume)) {
            app->state = STATE_PLAY;
        }
        else if (isMouseInside(mouseX, mouseY, btnPauseSettings)) {
            app->mouseReleased = false;
            app->state = STATE_SETTINGS;
        }
        else if (isMouseInside(mouseX, mouseY, btnPauseExit)) {
            app->mouseReleased = false;
            app->state = STATE_MENU;
            Mix_PlayMusic(app->menuMusic, -1);
        }
    }
}

void renderPauseMenu(App *app) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    int btnW = 200;
    int btnH = 60;
    int centerX = (WINDOW_WIDTH - btnW) / 2;

    SDL_Rect btnPauseResume     = { centerX, 250, btnW, btnH};
    SDL_Rect btnPauseSettings = { centerX, 350, btnW, btnH };
    SDL_Rect btnPauseExit     = { centerX, 450, btnW, btnH };

    // Малюємо кнопку Resume (Зелена)
    if (isMouseInside(mx, my, btnPauseResume)) SDL_SetRenderDrawColor(app->renderer, 100, 255, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 50, 200, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnPauseResume);

    // Малюємо кнопку SETTINGS (Жовта)
    if (isMouseInside(mx, my, btnPauseSettings)) SDL_SetRenderDrawColor(app->renderer, 255, 255, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 200, 200, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnPauseSettings);

    // Малюємо кнопку EXIT (Червона)
    if (isMouseInside(mx, my, btnPauseExit)) SDL_SetRenderDrawColor(app->renderer, 255, 100, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 200, 50, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnPauseExit);
}

// --- ЕКРАН GAME OVER ---
void updateGameOver(App *app, Level *level,Player *player) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    bool isLeftClicked = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));  
    
    if (!isLeftClicked) {
        app->mouseReleased = true;
    }

    int btnW = 200;
    int btnH = 60;
    int centerX = (WINDOW_WIDTH - btnW) / 2;

    SDL_Rect btnRestart = { centerX, 250, btnW, btnH };
    SDL_Rect btnQuit = { centerX, 340, btnW, btnH };
    if (keys[SDL_SCANCODE_RETURN] || (isLeftClicked && isMouseInside(mouseX, mouseY, btnRestart))) {
        initPlayer(player, level->spawnX, level->spawnY);
        app->state = STATE_PLAY;

        // --- РЕСТАРТ ТАЙМЕРА РІВНЯ ---
        app->gameStartTime = SDL_GetTicks();
    }
    if ((keys[SDL_SCANCODE_ESCAPE] || (isLeftClicked && isMouseInside(mouseX, mouseY, btnQuit))) && app->mouseReleased) {
        app->mouseReleased = false;
        initPlayer(player, level->spawnX, level->spawnY);
        app->state = STATE_MENU;
        Mix_PlayMusic(app->menuMusic, -1);
    }
}

void renderGameOver(App *app) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    int btnW = 200;
    int btnH = 60;
    int centerX = (WINDOW_WIDTH - btnW) / 2;

    SDL_Rect btnRestart = { centerX, 250, btnW, btnH };
    SDL_Rect btnQuit = { centerX, 340, btnW, btnH };

    if (isMouseInside(mouseX, mouseY, btnRestart)) SDL_SetRenderDrawColor(app->renderer, 100, 255, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 50, 200, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnRestart);

    if (isMouseInside(mouseX, mouseY, btnQuit)) SDL_SetRenderDrawColor(app->renderer, 255, 255, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 200, 200, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnQuit);
}
