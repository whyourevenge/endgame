#ifndef ENDGAME_H
#define ENDGAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 768

typedef enum {
    STATE_MENU,
    STATE_SETTINGS,
    STATE_PLAY,
    STATE_GAMEOVER
} GameState;

typedef struct {
    SDL_Window		*window;
    SDL_Renderer	*renderer;
    bool		    isRunning;
    GameState       state;
    int             currentLevel;
} App;

#endif
