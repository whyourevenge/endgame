#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>

// Розміри тайл-мапи
#define LEVEL_ROWS 24
#define LEVEL_COLS 40
#define TILE_SIZE 32

typedef struct {
    int map[LEVEL_ROWS][LEVEL_COLS];
} Level;

void initLevel(Level *level, int levelNumber);
void renderLevel(Level *level, SDL_Renderer *renderer);

#endif
