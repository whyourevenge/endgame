#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LEVEL_ROWS 24
#define LEVEL_COLS 40
#define TILE_SIZE 32

typedef enum {
    TILE_EMPTY = 0,
    TILE_WALL = 1,
    TILE_DANGER = 2,
    TILE_FINISH = 3,
    TILE_COIN = 4,
    TILE_SPAWN = 5
} TileType;

typedef struct {
    int map[LEVEL_ROWS][LEVEL_COLS];
    float spawnX;
    float spawnY;
} Level;

static void drawWall(Level *level, SDL_Renderer *renderer, int x, int y);
static void drawDanger(Level *level, SDL_Renderer *renderer, int x, int y);
static void drawFinish(Level *level, SDL_Renderer *renderer, int x, int y);
static void drawCoin(Level *level, SDL_Renderer *renderer, int x, int y);

void loadLevelFromFile(Level *level, const char *filename);
void initLevel(Level *level, int levelNumber);

void renderLevel(Level *level, SDL_Renderer *renderer);

#endif
