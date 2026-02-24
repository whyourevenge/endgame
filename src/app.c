#include "endgame.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

GameState previousState = STATE_MENU;

bool initGame(Game *game) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initialization SDL: %s\n", SDL_GetError());
        return false;
    }

    game->window = SDL_CreateWindow(
        "Endgame: Platformer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0
    );

    if (!game->window) {
        printf("Error create window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!game->renderer) {
        printf("Error create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return false;
    }
    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
        printf("Initialization error SDL_mixer: %s\n", Mix_GetError());
    else {
        game->menuMusic = Mix_LoadMUS("resource/audio/main-menu-music.ogg");
        if (!game->menuMusic) 
            printf("Failed to load music: %s\n", Mix_GetError());
        else {
            Mix_VolumeMusic(24);
            Mix_PlayMusic(game->menuMusic, -1);
        }
        
        game->level1Music = Mix_LoadMUS("resource/audio/level1.ogg");
        game->level2Music = Mix_LoadMUS("resource/audio/level2.ogg");
        game->level3Music = Mix_LoadMUS("resource/audio/level3.ogg");
    
        if (!game->level1Music || !game->level2Music || !game->level3Music) 
            printf("Warning: Failed to load music: for one of the levels. %s\n", Mix_GetError());
         
        game->winSound = Mix_LoadWAV("resource/audio/victory.wav");
        Mix_VolumeChunk(game->winSound, 24);
        if (!game->winSound) 
            printf("Failed to load victory sound: %s\n", Mix_GetError());
        
        game->coinSound = Mix_LoadWAV("resource/audio/coin.wav");
        Mix_VolumeChunk(game->coinSound, 24);
        if (!game->coinSound) 
            printf("Failed to load coin pickup sound: %s\n", Mix_GetError());

        app->gameOverSound = Mix_LoadWAV("resource/audio/gameover.wav");
        Mix_VolumeChunk(app->gameOverSound, 24);
        if(!app->gameOverSound)
            printf("Failed to load game over sound: %s\n", Mix_GetError());
    }

    game->menuBg = IMG_LoadTexture(game->renderer, "resource/images/menu_bg.png"); 
    if (!game->menuBg) {
        printf("Warning: Failed to load menu background: %s\n", IMG_GetError());
    }

    game->settingsBg = IMG_LoadTexture(game->renderer, "resource/images/settings_bg.png"); 
    if (!game->settingsBg) {
        printf("Warning: Failed to load settings background: %s\n", IMG_GetError());
    }

    game->victoryBg = IMG_LoadTexture(game->renderer, "resource/images/victory_bg.png");
    if (!game->victoryBg) 
        printf("Failed to load victory background: %s\n", IMG_GetError());
    
    game->playButton = IMG_LoadTexture(game->renderer, "resource/images/play-button.png");
    if (!game->playButton) 
        printf("Failed to load play button: %s\n", IMG_GetError());

    game->settingsButton = IMG_LoadTexture(game->renderer, "resource/images/settings-button.png");
    if (!game->settingsButton) 
        printf("Failed to load settings button: %s\n", IMG_GetError());
        
    game->exitButton = IMG_LoadTexture(game->renderer, "resource/images/exit-button.png");
    if (!game->exitButton) 
        printf("Failed to load exit button: %s\n", IMG_GetError());

    game->backButton = IMG_LoadTexture(game->renderer, "resource/images/back-button.png");
    if (!game->backButton) 
        printf("Failed to load back button: %s\n", IMG_GetError());
        
        
    game->resumeButton = IMG_LoadTexture(game->renderer, "resource/images/resume-button.png");
    if (!game->resumeButton) 
        printf("Failed to load resume button: %s\n", IMG_GetError());

    game->retryButton = IMG_LoadTexture(game->renderer, "resource/images/retry-button.png");
    if (!game->retryButton) 
        printf("Failed to load retry button: %s\n", IMG_GetError());

    if (TTF_Init() == -1) {
        printf("Initialization error SDL_ttf: %s\n", TTF_GetError());
        return false;
    }

    game->font = TTF_OpenFont("resource/Jersey10-Regular.ttf", 64); 
    if (!game->font) 
        printf("Failed to load font: %s\n", TTF_GetError());
    
    game->isRunning = true;
    return true;
}

void handleEvents(Game *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) 
            game->isRunning = false;
        
        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode key = event.key.keysym.sym;

            if (game->state == STATE_MENU) {
                if (key == SDLK_ESCAPE) 
                    game->isRunning = false;
            }
            else if (game->state == STATE_PLAY) {
                if (key == SDLK_ESCAPE) {
                    game->state = STATE_PAUSE;
                    Mix_PauseMusic();
                    previousState = game->state; 
                    game->mouseReleased = false;
                }
            }
            else if (game->state == STATE_PAUSE) {
                if (key == SDLK_ESCAPE) {
                    game->state = STATE_PLAY;
                    Mix_ResumeMusic(); 
                }
            }
            else if (game->state == STATE_SETTINGS) {
                if (key == SDLK_ESCAPE) 
                    game->state = previousState;
            }
           
            else if (game->state == STATE_VICTORY) {
                if (key == SDLK_KP_ENTER) {
                    game->state = STATE_MENU;
                    previousState = game->state; 
                }
            }
        }
    }
}

void cleanupGame(Game *game) {
    if (game->menuBg) 
        SDL_DestroyTexture(game->menuBg);

    if (game->settingsBg) 
        SDL_DestroyTexture(game->settingsBg);

    if (game->victoryBg) 
        SDL_DestroyTexture(game->victoryBg);

    if (game->playButton) 
        SDL_DestroyTexture(game->playButton);
        
    if (game->settingsButton) 
        SDL_DestroyTexture(game->settingsButton);

    if (game->exitButton) 
        SDL_DestroyTexture(game->exitButton);

    if (game->backButton) 
        SDL_DestroyTexture(game->backButton);
        
    if (game->resumeButton)
        SDL_DestroyTexture(game->resumeButton);
        
    if (game->retryButton)
        SDL_DestroyTexture(game->retryButton);
    
    if (game->menuMusic) 
        Mix_FreeMusic(game->menuMusic);
 
    if (game->winSound) 
        Mix_FreeChunk(game->winSound);
    
    if (game->coinSound) 
        Mix_FreeChunk(app->coinSound);

    if (game->gameOverSound) 
        Mix_FreeChunk(app->gameOverSound);
    
    if (game->level1Music) Mix_FreeMusic(game->level1Music);
    if (game->level2Music) Mix_FreeMusic(game->level2Music);
    if (game->level3Music) Mix_FreeMusic(game->level3Music);
    
    Mix_CloseAudio();

    if (game->font) TTF_CloseFont(game->font);
    TTF_Quit();

    if (game->renderer) 
        SDL_DestroyRenderer(game->renderer);
    
    if (game->window) 
        SDL_DestroyWindow(game->window);
        
    SDL_Quit();
    printf("Game closed successfully. Resources freed.\n");
}
