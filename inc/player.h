#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "endgame.h"
#include "level.h"


typedef enum { 
    GRAV_DOWN, 
    GRAV_UP, 
    GRAV_LEFT, 
    GRAV_RIGHT 
} GravityDir;

typedef struct {
    float x, y;
    float dx, dy;
    int width, height;
    GravityDir gravDir;
    Uint32 spawnTime;
} Player;

void initPlayer(Player *p, float spawnX, float spawnY);
void updatePlayer(Player *p, Level *level, App *app);
void renderPlayer(Player *p, SDL_Renderer *renderer);

#endif
