#include "endgame.h"
#include "player.h"
#include "level.h"
#include "menu.h"
#include "utilities.h"

int main(void) {
    Game game = {0};

    if (!initGame(&game)) 
        return 1;
    
    game.state = STATE_MENU;
    game.currentLevel = 1;

    Player player = {0};
    Level level = {0};

    initLevel(&level, game.currentLevel);
    initPlayer(&player, level.spawnX, level.spawnY);

    const int FPS = 60;
    const int frameDelay = 1000 / FPS; 
    Uint32 frameStart;
    int frameTime;

    while (game.isRunning) {
        frameStart = SDL_GetTicks();
        
        handleEvents(&game);

        switch (game.state) {
            case STATE_MENU:
                updateMenu(&game, &player);
                break;
            case STATE_PLAY:
                updatePlayer(&player, &level, &game); 
                break;
            case STATE_PAUSE:
                updatePauseMenu(&game, &level, &player);
                break;
            case STATE_GAMEOVER:
                player.coins = player.coinsAtLevelStart;
                updateGameOver(&game, &level, &player, game.currentLevel, game.renderer);
                if (game.gameOverAlpha < 150) 
                    game.gameOverAlpha += 5;
                break;
            case STATE_SETTINGS:
                updateSettings(&game);
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE]) 
                    game.state = STATE_MENU;
                break;
            case STATE_VICTORY:
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RETURN] || 
                    SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE]) {
                    resetGame(&game, &level, &player);
                    game.state = STATE_MENU;
                    Mix_PlayMusic(game.menuMusic, -1);
                }
                break;
        }

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game.renderer);

        switch (game.state) {
            case STATE_MENU:
                renderMenu(&game);
                break;
            case STATE_PLAY:
                renderLevel(&level, game.renderer);
                renderPlayer(&player, game.renderer);
                break;
            case STATE_PAUSE:
                renderLevel(&level, game.renderer);
                renderPlayer(&player, game.renderer);
                SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 150); 
                SDL_Rect overlay = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
                SDL_RenderFillRect(game.renderer, &overlay);
                renderPauseMenu(&game); 
                break;
            case STATE_GAMEOVER:
                renderLevel(&level, game.renderer);
                renderPlayer(&player, game.renderer);
                SDL_SetRenderDrawColor(game.renderer, 100, 0, 0, game.gameOverAlpha); 
                SDL_Rect fullScreen = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
                SDL_RenderFillRect(game.renderer, &fullScreen);
                if (game.gameOverAlpha >= 150) 
                    renderGameOver(&game);
                break;
            case STATE_SETTINGS:
                renderSettings(&game);
                break;
            case STATE_VICTORY:
                if (game.victoryBg != NULL) 
                    SDL_RenderCopy(game.renderer, game.victoryBg, NULL, NULL);
                else {
                    SDL_SetRenderDrawColor(game.renderer, 255, 215, 0, 255);
                    SDL_RenderClear(game.renderer);
                }

                if (game.font != NULL) {
                    char timeText[128];
                    sprintf(timeText, "VICTORY! Time: %.2f sec | Deaths: %d", game.finalTime, game.deathCount);

                    SDL_Color whiteColor = {255, 255, 255, 255}; 
                    SDL_Surface *surf1 = TTF_RenderUTF8_Blended(game.font, timeText, whiteColor);
                    SDL_Texture *tex1 = SDL_CreateTextureFromSurface(game.renderer, surf1);

                    SDL_Rect rect1 = { 
                        (WINDOW_WIDTH - surf1->w) / 2, 
                        (WINDOW_HEIGHT / 2) - surf1->h, 
                        surf1->w, 
                        surf1->h 
                    };
                    SDL_RenderCopy(game.renderer, tex1, NULL, &rect1);

                    const char *hintText = "Press [ENTER] to return to Main Menu";
                    SDL_Color grayColor = {200, 200, 200, 255}; 
                    SDL_Surface *surf2 = TTF_RenderUTF8_Blended(game.font, hintText, grayColor);
                    SDL_Texture *tex2 = SDL_CreateTextureFromSurface(game.renderer, surf2);

                    SDL_Rect rect2 = { 
                        (WINDOW_WIDTH - surf2->w) / 2, 
                        (WINDOW_HEIGHT / 2) + 20, 
                        surf2->w, 
                        surf2->h 
                    };
                    SDL_RenderCopy(game.renderer, tex2, NULL, &rect2);

                    SDL_FreeSurface(surf1);
                    SDL_DestroyTexture(tex1);
                    SDL_FreeSurface(surf2);
                    SDL_DestroyTexture(tex2);
                }
                break;
        }

        SDL_RenderPresent(game.renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) 
            SDL_Delay(frameDelay - frameTime);
    }

    cleanupGame(&game);
    return 0;
}
