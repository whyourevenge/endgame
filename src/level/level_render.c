#include "../../inc/level.h"

static void drawWall(Level *level, SDL_Renderer *renderer, int x, int y) {
    SDL_Rect rect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &rect);
}

static void drawDanger(Level *level, SDL_Renderer *renderer, int x, int y) {
    SDL_Rect rect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

static void drawFinish(Level *level, SDL_Renderer *renderer, int x, int y) {
    SDL_Rect rect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

static void drawCoin(Level *level, SDL_Renderer *renderer, int x, int y) {
    SDL_Rect rect = { x * TILE_SIZE + 8, y * TILE_SIZE + 8, TILE_SIZE - 16, TILE_SIZE - 16 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void renderLevel(Level *level, SDL_Renderer *renderer) {
    for (int y = 0; y < LEVEL_ROWS; y++) {
        for (int x = 0; x < LEVEL_COLS; x++) {
            int tile = level->map[y][x];
            
            switch (tile) {
                case TILE_WALL:   drawWall(level, renderer, x, y);   break;   
                case TILE_DANGER: drawDanger(level, renderer, x, y); break;
                case TILE_FINISH: drawFinish(level, renderer, x, y); break;
                case TILE_COIN:   drawCoin(level, renderer, x, y);   break; 
                default: break;
            }
        }
    }
}