#ifndef ENDGAME_H
#define ENDGAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 768

typedef enum {
    STATE_MENU,
    STATE_SETTINGS,
    STATE_PLAY,
    STATE_PAUSE,
    STATE_GAMEOVER,
    STATE_VICTORY
} GameState;

typedef struct {
    SDL_Window		*window;
    SDL_Renderer	*renderer;
    bool		    isRunning;
    bool            mouseReleased;
    GameState       state;
    int             currentLevel;
    int             gameOverAlpha;
    int             deathCount;
    Uint32          levelStartTime;
    Mix_Music       *menuMusic;
} App;

#endif
