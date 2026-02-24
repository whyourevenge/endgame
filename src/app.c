#include "endgame.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

GameState previousState = STATE_MENU;

bool initApp(App *app) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initialization SDL: %s\n", SDL_GetError());
        return false;
    }

    app->window = SDL_CreateWindow(
        "Endgame: Platformer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0
    );

    if (!app->window) {
        printf("Error create window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!app->renderer) {
        printf("Error create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        return false;
    }
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
        printf("Initialization error SDL_mixer: %s\n", Mix_GetError());
    else {
        app->menuMusic = Mix_LoadMUS("resource/audio/main-menu-music.ogg");
        if (!app->menuMusic) 
            printf("Failed to load music: %s\n", Mix_GetError());
        else {
            Mix_VolumeMusic(24);
            Mix_PlayMusic(app->menuMusic, -1);
        }
        
        app->level1Music = Mix_LoadMUS("resource/audio/level1.ogg");
        app->level2Music = Mix_LoadMUS("resource/audio/level2.ogg");
        app->level3Music = Mix_LoadMUS("resource/audio/level3.ogg");
    
        if (!app->level1Music || !app->level2Music || !app->level3Music) 
            printf("Warning: Failed to load music: for one of the levels. %s\n", Mix_GetError());
         
        app->winSound = Mix_LoadWAV("resource/audio/victory.wav");
        Mix_VolumeChunk(app->winSound, 24);
        if (!app->winSound) 
            printf("Failed to load victory sound: %s\n", Mix_GetError());
        
        app->coinSound = Mix_LoadWAV("resource/audio/coin.wav");
        Mix_VolumeChunk(app->coinSound, 24);
        if (!app->coinSound) 
            printf("Failed to load coin pickup sound: %s\n", Mix_GetError());
    }

    app->menuBg = IMG_LoadTexture(app->renderer, "resource/images/menu_bg.png"); 
    if (!app->menuBg) {
        printf("Warning: Failed to load menu background: %s\n", IMG_GetError());
    }

    app->settingsBg = IMG_LoadTexture(app->renderer, "resource/images/settings_bg.png"); 
    if (!app->settingsBg) {
        printf("Warning: Failed to load settings background: %s\n", IMG_GetError());
    }

    app->victoryBg = IMG_LoadTexture(app->renderer, "resource/images/victory_bg.png");
    if (!app->victoryBg) 
        printf("Failed to load victory background: %s\n", IMG_GetError());
    
    app->playButton = IMG_LoadTexture(app->renderer, "resource/images/play-button.png");
    if (!app->playButton) 
        printf("Failed to load play button: %s\n", IMG_GetError());

    app->settingsButton = IMG_LoadTexture(app->renderer, "resource/images/settings-button.png");
    if (!app->settingsButton) 
        printf("Failed to load settings button: %s\n", IMG_GetError());
        
    app->exitButton = IMG_LoadTexture(app->renderer, "resource/images/exit-button.png");
    if (!app->exitButton) 
        printf("Failed to load exit button: %s\n", IMG_GetError());

    app->backButton = IMG_LoadTexture(app->renderer, "resource/images/back-button.png");
    if (!app->backButton) 
        printf("Failed to load back button: %s\n", IMG_GetError());
        
        
    app->resumeButton = IMG_LoadTexture(app->renderer, "resource/images/resume-button.png");
    if (!app->resumeButton) 
        printf("Failed to load resume button: %s\n", IMG_GetError());

    app->retryButton = IMG_LoadTexture(app->renderer, "resource/images/retry-button.png");
    if (!app->retryButton) 
        printf("Failed to load retry button: %s\n", IMG_GetError());

    if (TTF_Init() == -1) {
        printf("Initialization error SDL_ttf: %s\n", TTF_GetError());
        return false;
    }

    app->font = TTF_OpenFont("resource/Jersey10-Regular.ttf", 64); 
    if (!app->font) 
        printf("Failed to load font: %s\n", TTF_GetError());
    
    app->isRunning = true;
    return true;
}

void handleEvents(App *app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) 
            app->isRunning = false;
        
        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode key = event.key.keysym.sym;

            if (app->state == STATE_MENU) {
                if (key == SDLK_ESCAPE) 
                    app->isRunning = false;
            }
            else if (app->state == STATE_PLAY) {
                if (key == SDLK_ESCAPE) {
                    app->state = STATE_PAUSE;
                    Mix_PauseMusic();
                    previousState = app->state; 
                    app->mouseReleased = false;
                }
            }
            else if (app->state == STATE_PAUSE) {
                if (key == SDLK_ESCAPE) {
                    app->state = STATE_PLAY;
                    Mix_ResumeMusic(); 
                }
            }
            else if (app->state == STATE_SETTINGS) {
                if (key == SDLK_ESCAPE) 
                    app->state = previousState;
            }
            else if (app->state == STATE_GAMEOVER) {
                if (key == SDLK_KP_ENTER) 
                    app->state = STATE_PLAY;
                else if (key == SDLK_ESCAPE) {
                    app->state = STATE_MENU;
                    previousState = app->state; 
                }
            }
            else if (app->state == STATE_VICTORY) {
                if (key == SDLK_KP_ENTER) {
                    app->state = STATE_MENU;
                    previousState = app->state; 
                }
            }
        }
    }
}

void cleanupApp(App *app) {
    if (app->menuBg) 
        SDL_DestroyTexture(app->menuBg);

    if (app->settingsBg) 
        SDL_DestroyTexture(app->settingsBg);

    if (app->victoryBg) 
        SDL_DestroyTexture(app->victoryBg);

    if (app->playButton) 
        SDL_DestroyTexture(app->playButton);
        
    if (app->settingsButton) 
        SDL_DestroyTexture(app->settingsButton);

    if (app->exitButton) 
        SDL_DestroyTexture(app->exitButton);

    if (app->backButton) 
        SDL_DestroyTexture(app->backButton);
        
    if (app->resumeButton)
        SDL_DestroyTexture(app->resumeButton);
        
    if (app->retryButton)
        SDL_DestroyTexture(app->retryButton);
    
    if (app->menuMusic) 
        Mix_FreeMusic(app->menuMusic);
 
    if (app->winSound) 
        Mix_FreeChunk(app->winSound);
    
    if (app->coinSound) 
        Mix_FreeChunk(app->coinSound);
    
    if (app->level1Music) Mix_FreeMusic(app->level1Music);
    if (app->level2Music) Mix_FreeMusic(app->level2Music);
    if (app->level3Music) Mix_FreeMusic(app->level3Music);
    
    Mix_CloseAudio();

    if (app->font) TTF_CloseFont(app->font);
    TTF_Quit();

    if (app->renderer) 
        SDL_DestroyRenderer(app->renderer);
    
    if (app->window) 
        SDL_DestroyWindow(app->window);
        
    SDL_Quit();
    printf("Game closed successfully. Resources freed.\n");
}
