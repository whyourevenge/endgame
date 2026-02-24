#include "endgame.h"
#include "player.h"
#include "level.h"
#include "menu.h"
#include "utilities.h"

int main(void) {
    App app = {0};

    if (!initApp(&app)) 
        return 1;
    
    app.state = STATE_MENU;
    app.currentLevel = 1;

    Player player = {0};
    Level level = {0};

    initLevel(&level, app.currentLevel);
    initPlayer(&player, level.spawnX, level.spawnY);

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; 
    Uint32 frameStart;
    int frameTime;

    while (app.isRunning) {
        frameStart = SDL_GetTicks();
        
        handleEvents(&app);

        switch (app.state) {
            case STATE_MENU:
                updateMenu(&app, &player);
                break;
            case STATE_PLAY:
                updatePlayer(&player, &level, &app); 
                break;
            case STATE_PAUSE:
                updatePauseMenu(&app, &level, &player);
                break;
            case STATE_GAMEOVER:
                player.coins = player.coinsAtLevelStart;
                updateGameOver(&app, &level, &player, app.currentLevel, app.renderer);
                if (app.gameOverAlpha < 150) 
                    app.gameOverAlpha += 3;
                break;
            case STATE_SETTINGS:
                updateSettings(&app);
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE]) 
                    app.state = STATE_MENU;
                break;
            case STATE_VICTORY:
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RETURN] || 
                    SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE]) {
                    resetGame(&app, &level, &player);
                    app.state = STATE_MENU;
                    Mix_PlayMusic(app.menuMusic, -1);
                }
                break;
        }

        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);

        switch (app.state) {
            case STATE_MENU:
                renderMenu(&app);
                break;
            case STATE_PLAY:
                renderLevel(&level, app.renderer);
                renderPlayer(&player, app.renderer);
                break;
            case STATE_PAUSE:
                renderLevel(&level, app.renderer);
                renderPlayer(&player, app.renderer);
                SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 150); 
                SDL_Rect overlay = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
                SDL_RenderFillRect(app.renderer, &overlay);
                renderPauseMenu(&app); 
                break;
            case STATE_GAMEOVER:
                renderLevel(&level, app.renderer);
                renderPlayer(&player, app.renderer);
                SDL_SetRenderDrawColor(app.renderer, 100, 0, 0, app.gameOverAlpha); 
                SDL_Rect fullScreen = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
                SDL_RenderFillRect(app.renderer, &fullScreen);
                if (app.gameOverAlpha >= 150) 
                    renderGameOver(&app);
                break;
            case STATE_SETTINGS:
                renderSettings(&app);
                break;
            case STATE_VICTORY:
                if (app.victoryBg != NULL) 
                    SDL_RenderCopy(app.renderer, app.victoryBg, NULL, NULL);
                else {
                    SDL_SetRenderDrawColor(app.renderer, 255, 215, 0, 255);
                    SDL_RenderClear(app.renderer);
                }

                if (app.font != NULL) {
                    char timeText[128];
                    sprintf(timeText, "VICTORY! Time: %.2f sec | Deaths: %d", app.finalTime, app.deathCount);

                    SDL_Color whiteColor = {255, 255, 255, 255}; 
                    SDL_Surface *surf1 = TTF_RenderUTF8_Blended(app.font, timeText, whiteColor);
                    SDL_Texture *tex1 = SDL_CreateTextureFromSurface(app.renderer, surf1);

                    SDL_Rect rect1 = { 
                        (WINDOW_WIDTH - surf1->w) / 2, 
                        (WINDOW_HEIGHT / 2) - surf1->h, 
                        surf1->w, 
                        surf1->h 
                    };
                    SDL_RenderCopy(app.renderer, tex1, NULL, &rect1);

                    const char *hintText = "Press [ENTER] to return to Main Menu";
                    SDL_Color grayColor = {200, 200, 200, 255}; 
                    SDL_Surface *surf2 = TTF_RenderUTF8_Blended(app.font, hintText, grayColor);
                    SDL_Texture *tex2 = SDL_CreateTextureFromSurface(app.renderer, surf2);

                    SDL_Rect rect2 = { 
                        (WINDOW_WIDTH - surf2->w) / 2, 
                        (WINDOW_HEIGHT / 2) + 20, 
                        surf2->w, 
                        surf2->h 
                    };
                    SDL_RenderCopy(app.renderer, tex2, NULL, &rect2);

                    SDL_FreeSurface(surf1);
                    SDL_DestroyTexture(tex1);
                    SDL_FreeSurface(surf2);
                    SDL_DestroyTexture(tex2);
                }
                break;
        }

        SDL_RenderPresent(app.renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) 
            SDL_Delay(frameDelay - frameTime);
    }

    cleanupApp(&app);
    return 0;
}
