#ifndef ENDGAME_H
#define ENDGAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 768

typedef enum {
    STATE_MENU,
    STATE_SETTINGS,
    STATE_PLAY,
    STATE_GAMEOVER,
    STATE_VICTORY
} GameState;

typedef struct {
    SDL_Window		*window;
    SDL_Renderer	*renderer;
    bool		    isRunning;
    GameState       state;
    int             currentLevel;

    int             deathCount;
    Uint32          gameStartTime;

    Mix_Music       *menuMusic;
    Mix_Chunk       *winSound;

    float           finalTime;     // Щоб зберегти фінальний час
    TTF_Font        *font;         // Наш шрифт

    SDL_Texture     *victoryBg;
} App;

#endif
