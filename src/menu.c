#include <SDL2/SDL.h>
#include "endgame.h"
#include "menu.h"
#include "player.h"
#include "utilities.h"

bool isMouseInside(int mouseX, int mouseY, SDL_Rect rect) {
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

void updateMenu(Game *game, Player *player) {
    int mx, my;
    Uint32 mouseState = SDL_GetMouseState(&mx, &my);
    bool isLeftClicked = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

    if (!isLeftClicked) 
        game->mouseReleased = true;
    
    SDL_Rect btnPlay     = { 1280/2 - 100, 250, 200, 60 };
    SDL_Rect btnSettings = { 1280/2 - 100, 350, 200, 60 };
    SDL_Rect btnExit     = { 1280/2 - 100, 450, 200, 60 };

    if (isLeftClicked && game->mouseReleased) {
        if (isMouseInside(mx, my, btnPlay)) {
            Mix_HaltMusic();
            game->state = STATE_PLAY; 

            game->state = STATE_PLAY;
            game->currentLevel = 1;
            game->deathCount = 0;
            player->coins = 0;
            game->gameStartTime = SDL_GetTicks();
            
            if (game->level1Music) 
                Mix_PlayMusic(game->level1Music, -1);

        }
        
        else if (isMouseInside(mx, my, btnSettings)) {
            game->mouseReleased = false;
            previousState = game->state;
            game->state = STATE_SETTINGS;
        }
        else if (isMouseInside(mx, my, btnExit)) 
            game->isRunning = false; 
    }
}

void renderMenu(Game *game) {
    if (game->menuBg != NULL) {
        SDL_RenderCopy(game->renderer, game->menuBg, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(game->renderer, 20, 20, 30, 255);
        SDL_RenderClear(game->renderer);
    }

    int mx, my;
    SDL_GetMouseState(&mx, &my);

    SDL_Rect btnPlay     = { 1280/2 - 100, 250, 200, 60 };
    SDL_Rect btnSettings = { 1280/2 - 100, 350, 200, 60 };
    SDL_Rect btnExit     = { 1280/2 - 100, 450, 200, 60 };

    if (game->playButton) {
        if (isMouseInside(mx, my, btnPlay)) {
            SDL_SetTextureColorMod(game->playButton, 255, 255, 255);
        } else {
            SDL_SetTextureColorMod(game->playButton, 200, 200, 200);
        }
        SDL_RenderCopy(game->renderer, game->playButton, NULL, &btnPlay);
    } else {
        if (isMouseInside(mx, my, btnPlay)) SDL_SetRenderDrawColor(game->renderer, 100, 255, 100, 255);
        else SDL_SetRenderDrawColor(game->renderer, 50, 200, 50, 255);
        SDL_RenderFillRect(game->renderer, &btnPlay);
    }

    if (game->settingsButton) {
        if (isMouseInside(mx, my, btnSettings)) {
            SDL_SetTextureColorMod(game->settingsButton, 255, 255, 255);
        } else {
            SDL_SetTextureColorMod(game->settingsButton, 200, 200, 200);
        }
        SDL_RenderCopy(game->renderer, game->settingsButton, NULL, &btnSettings);
    } else {
        if (isMouseInside(mx, my, btnSettings)) SDL_SetRenderDrawColor(game->renderer, 100, 255, 100, 255);
        else SDL_SetRenderDrawColor(game->renderer, 50, 200, 50, 255);
        SDL_RenderFillRect(game->renderer, &btnSettings);
    }

    if (game->exitButton) {
        if (isMouseInside(mx, my, btnExit)) {
            SDL_SetTextureColorMod(game->exitButton, 255, 255, 255);
        } else {
            SDL_SetTextureColorMod(game->exitButton, 200, 200, 200);
        }
        SDL_RenderCopy(game->renderer, game->exitButton, NULL, &btnExit);
    } else {
        if (isMouseInside(mx, my, btnExit)) SDL_SetRenderDrawColor(game->renderer, 100, 255, 100, 255);
        else SDL_SetRenderDrawColor(game->renderer, 50, 200, 50, 255);
        SDL_RenderFillRect(game->renderer, &btnExit);
    }
}

void updatePauseMenu(Game *game, Level *level, Player *player) {
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    bool isLeftClicked = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

    if (!isLeftClicked) 
        game->mouseReleased = true;
   
    int btnW = 200;
    int btnH = 60;
    int centerX = (WINDOW_WIDTH - btnW) / 2;

    SDL_Rect btnPauseResume     = { centerX, 250, btnW, btnH};
    SDL_Rect btnPauseSettings = { centerX, 350, btnW, btnH };
    SDL_Rect btnPauseExit     = { centerX, 450, btnW, btnH };

    if (isLeftClicked && game->mouseReleased) {
        if (isMouseInside(mouseX, mouseY, btnPauseResume)) {
            game->state = STATE_PLAY;
            Mix_ResumeMusic();
        }
        else if (isMouseInside(mouseX, mouseY, btnPauseSettings)) {
            game->mouseReleased = false;
            previousState = game->state;
            game->state = STATE_SETTINGS;
        }
        else if (isMouseInside(mouseX, mouseY, btnPauseExit)) {
            game->mouseReleased = false;
            game->state = STATE_MENU;
            resetGame(game, level, player);
            Mix_PlayMusic(game->menuMusic, -1);
        }
    }
}

void renderPauseMenu(Game *game) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    int btnW = 200;
    int btnH = 60;
    int centerX = (WINDOW_WIDTH - btnW) / 2;

    SDL_Rect btnPauseResume     = { centerX, 250, btnW, btnH};
    SDL_Rect btnPauseSettings = { centerX, 350, btnW, btnH };
    SDL_Rect btnPauseExit     = { centerX, 450, btnW, btnH };

    if (game->resumeButton) {
        bool hovered = isMouseInside(mx, my, btnPauseResume);
        SDL_SetTextureColorMod(game->resumeButton, hovered ? 255 : 200, hovered ? 255 : 200, hovered ? 255 : 200);
        SDL_RenderCopy(game->renderer, game->resumeButton, NULL, &btnPauseResume);
    }

    if (game->settingsButton) {
        bool hovered = isMouseInside(mx, my, btnPauseSettings);
        SDL_SetTextureColorMod(game->settingsButton, hovered ? 255 : 200, hovered ? 255 : 200, hovered ? 255 : 200);
        SDL_RenderCopy(game->renderer, game->settingsButton, NULL, &btnPauseSettings);
    }

    if (game->exitButton) {
        bool hovered = isMouseInside(mx, my, btnPauseExit);
        SDL_SetTextureColorMod(game->exitButton, hovered ? 255 : 200, hovered ? 255 : 200, hovered ? 255 : 200);
        SDL_RenderCopy(game->renderer, game->exitButton, NULL, &btnPauseExit);
    }
}

void updateGameOver(Game *game, Level *level, Player *player, int currentLevel, SDL_Renderer *renderer) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    bool isLeftClicked = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));  
    
    if (!isLeftClicked) 
        game->mouseReleased = true;
    
    int btnW = 200;
    int btnH = 60;
    int centerX = (WINDOW_WIDTH - btnW) / 2;

    SDL_Rect btnRestart = { centerX, 325, btnW, btnH };
    SDL_Rect btnQuit = { centerX, 415, btnW, btnH };
    if (keys[SDL_SCANCODE_RETURN] || (isLeftClicked && isMouseInside(mouseX, mouseY, btnRestart))) {
        initPlayer(player, level->spawnX, level->spawnY);
        initLevel(level, currentLevel);
        renderLevel(level, renderer);
        game->state = STATE_PLAY;


        
        if (game->currentLevel == 1 && game->level1Music) Mix_PlayMusic(game->level1Music, -1);
        else if (game->currentLevel == 2 && game->level2Music) Mix_PlayMusic(game->level2Music, -1);
        else if (game->currentLevel == 3 && game->level3Music) Mix_PlayMusic(game->level3Music, -1);
    }
    if ((keys[SDL_SCANCODE_ESCAPE] || (isLeftClicked && isMouseInside(mouseX, mouseY, btnQuit))) && game->mouseReleased) {
        game->mouseReleased = false;
        initPlayer(player, level->spawnX, level->spawnY);
        game->state = STATE_MENU;
        resetGame(game, level, player);
        Mix_PlayMusic(game->menuMusic, -1);
    }
}

void renderGameOver(Game *game) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    int btnW = 200;
    int btnH = 60;
    int centerX = (WINDOW_WIDTH - btnW) / 2;

    SDL_Rect btnRestart = { centerX, 325, btnW, btnH };
    SDL_Rect btnQuit = { centerX, 415, btnW, btnH };

    if (game->retryButton) {
        bool hovered = isMouseInside(mouseX, mouseY, btnRestart);
        SDL_SetTextureColorMod(game->retryButton, hovered ? 255 : 200, hovered ? 255 : 200, hovered ? 255 : 200);
        SDL_RenderCopy(game->renderer, game->retryButton, NULL, &btnRestart);
    }

    if (game->exitButton) {
        bool hovered = isMouseInside(mouseX, mouseY, btnQuit);
        SDL_SetTextureColorMod(game->exitButton, hovered ? 255 : 200, hovered ? 255 : 200, hovered ? 255 : 200);
        SDL_RenderCopy(game->renderer, game->exitButton, NULL, &btnQuit);
    }
}
