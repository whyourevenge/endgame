#include <SDL2/SDL.h>
#include "endgame.h"
#include "menu.h"
#include "player.h"
#include "utilities.h"

bool isMouseInside(int mouseX, int mouseY, SDL_Rect rect) {
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

void updateMenu(App *app, Player *player) {
    int mx, my;
    Uint32 mouseState = SDL_GetMouseState(&mx, &my);
    bool isLeftClicked = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

    if (!isLeftClicked) 
        app->mouseReleased = true;
    
    SDL_Rect btnPlay     = { 1280/2 - 100, 250, 200, 60 };
    SDL_Rect btnSettings = { 1280/2 - 100, 350, 200, 60 };
    SDL_Rect btnExit     = { 1280/2 - 100, 450, 200, 60 };

    if (isLeftClicked && app->mouseReleased) {
        if (isMouseInside(mx, my, btnPlay)) {
            Mix_HaltMusic();
            app->state = STATE_PLAY; 

            app->state = STATE_PLAY;
            app->currentLevel = 1;
            app->deathCount = 0;
            player->coins = 0;
            app->gameStartTime = SDL_GetTicks();
            
            if (app->level1Music) 
                Mix_PlayMusic(app->level1Music, -1);

        }
        
        else if (isMouseInside(mx, my, btnSettings)) {
            app->mouseReleased = false;
            app->state = STATE_SETTINGS;
        }
        else if (isMouseInside(mx, my, btnExit)) 
            app->isRunning = false; 
    }
}

void renderMenu(App *app) {
    if (app->menuBg != NULL) {
        SDL_RenderCopy(app->renderer, app->menuBg, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(app->renderer, 20, 20, 30, 255);
        SDL_RenderClear(app->renderer);
    }

    int mx, my;
    SDL_GetMouseState(&mx, &my);

    SDL_Rect btnPlay     = { 1280/2 - 100, 250, 200, 60 };
    SDL_Rect btnSettings = { 1280/2 - 100, 350, 200, 60 };
    SDL_Rect btnExit     = { 1280/2 - 100, 450, 200, 60 };

    if (app->playButton) {
        if (isMouseInside(mx, my, btnPlay)) {
            SDL_SetTextureColorMod(app->playButton, 255, 255, 255);
        } else {
            SDL_SetTextureColorMod(app->playButton, 200, 200, 200);
        }
        SDL_RenderCopy(app->renderer, app->playButton, NULL, &btnPlay);
    } else {
        if (isMouseInside(mx, my, btnPlay)) SDL_SetRenderDrawColor(app->renderer, 100, 255, 100, 255);
        else SDL_SetRenderDrawColor(app->renderer, 50, 200, 50, 255);
        SDL_RenderFillRect(app->renderer, &btnPlay);
    }

    if (app->settingsButton) {
        if (isMouseInside(mx, my, btnSettings)) {
            SDL_SetTextureColorMod(app->settingsButton, 255, 255, 255);
        } else {
            SDL_SetTextureColorMod(app->settingsButton, 200, 200, 200);
        }
        SDL_RenderCopy(app->renderer, app->settingsButton, NULL, &btnSettings);
    } else {
        if (isMouseInside(mx, my, btnSettings)) SDL_SetRenderDrawColor(app->renderer, 100, 255, 100, 255);
        else SDL_SetRenderDrawColor(app->renderer, 50, 200, 50, 255);
        SDL_RenderFillRect(app->renderer, &btnSettings);
    }

    if (app->exitButton) {
        if (isMouseInside(mx, my, btnExit)) {
            SDL_SetTextureColorMod(app->exitButton, 255, 255, 255);
        } else {
            SDL_SetTextureColorMod(app->exitButton, 200, 200, 200);
        }
        SDL_RenderCopy(app->renderer, app->exitButton, NULL, &btnExit);
    } else {
        if (isMouseInside(mx, my, btnExit)) SDL_SetRenderDrawColor(app->renderer, 100, 255, 100, 255);
        else SDL_SetRenderDrawColor(app->renderer, 50, 200, 50, 255);
        SDL_RenderFillRect(app->renderer, &btnExit);
    }
}

void updatePauseMenu(App *app, Level *level, Player *player) {
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    bool isLeftClicked = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

    if (!isLeftClicked) 
        app->mouseReleased = true;
   
    int btnW = 200;
    int btnH = 60;
    int centerX = (WINDOW_WIDTH - btnW) / 2;

    SDL_Rect btnPauseResume     = { centerX, 250, btnW, btnH};
    SDL_Rect btnPauseSettings = { centerX, 350, btnW, btnH };
    SDL_Rect btnPauseExit     = { centerX, 450, btnW, btnH };

    if (isLeftClicked && app->mouseReleased) {
        if (isMouseInside(mouseX, mouseY, btnPauseResume)) {
            app->state = STATE_PLAY;
            Mix_ResumeMusic();
        }
        else if (isMouseInside(mouseX, mouseY, btnPauseSettings)) {
            app->mouseReleased = false;
            app->state = STATE_SETTINGS;
        }
        else if (isMouseInside(mouseX, mouseY, btnPauseExit)) {
            app->mouseReleased = false;
            app->state = STATE_MENU;
            resetGame(app, level, player);
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

    if (isMouseInside(mx, my, btnPauseResume)) SDL_SetRenderDrawColor(app->renderer, 100, 255, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 50, 200, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnPauseResume);

    if (isMouseInside(mx, my, btnPauseSettings)) SDL_SetRenderDrawColor(app->renderer, 255, 255, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 200, 200, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnPauseSettings);

    if (isMouseInside(mx, my, btnPauseExit)) SDL_SetRenderDrawColor(app->renderer, 255, 100, 100, 255);
    else SDL_SetRenderDrawColor(app->renderer, 200, 50, 50, 255);
    SDL_RenderFillRect(app->renderer, &btnPauseExit);
}

void updateGameOver(App *app, Level *level,Player *player, int CurrentLevel, SDL_Renderer *renderer) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    bool isLeftClicked = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));  
    
    if (!isLeftClicked) 
        app->mouseReleased = true;
    
    int btnW = 200;
    int btnH = 60;
    int centerX = (WINDOW_WIDTH - btnW) / 2;

    SDL_Rect btnRestart = { centerX, 250, btnW, btnH };
    SDL_Rect btnQuit = { centerX, 340, btnW, btnH };
    if (keys[SDL_SCANCODE_RETURN] || (isLeftClicked && isMouseInside(mouseX, mouseY, btnRestart))) {
        initPlayer(player, level->spawnX, level->spawnY);
        initLevel(level, CurrentLevel);
        renderLevel(level, renderer);
        app->state = STATE_PLAY;

        app->gameStartTime = SDL_GetTicks();
        
        if (app->currentLevel == 1 && app->level1Music) Mix_PlayMusic(app->level1Music, -1);
        else if (app->currentLevel == 2 && app->level2Music) Mix_PlayMusic(app->level2Music, -1);
        else if (app->currentLevel == 3 && app->level3Music) Mix_PlayMusic(app->level3Music, -1);
    }
    if ((keys[SDL_SCANCODE_ESCAPE] || (isLeftClicked && isMouseInside(mouseX, mouseY, btnQuit))) && app->mouseReleased) {
        app->mouseReleased = false;
        initPlayer(player, level->spawnX, level->spawnY);
        app->state = STATE_MENU;
        resetGame(app, level, player);
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