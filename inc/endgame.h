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
    STATE_PAUSE,
    STATE_GAMEOVER,
    STATE_VICTORY
} GameState;

extern GameState currentState;
extern GameState previousState;

typedef struct {
    SDL_Window		*window;
    SDL_Renderer	*renderer;
    bool		    isRunning;
    bool            mouseReleased;
    GameState       state;
    int             currentLevel;
    int             gameOverAlpha;
    int             deathCount;
  
    Uint32          gameStartTime;
  
    Mix_Music       *menuMusic;
    Mix_Chunk       *winSound;
    Mix_Chunk       *coinSound;
    
    
    Mix_Music       *level1Music;
    Mix_Music       *level2Music;
    Mix_Music       *level3Music;

    float           finalTime;     // Щоб зберегти фінальний час
    TTF_Font        *font;         // Наш шрифт

    SDL_Texture     *victoryBg;
} App;

#endif
