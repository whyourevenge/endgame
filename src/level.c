#include "../inc/level.h"

void loadLevelFromFile(Level *level, const char *filename) {
    memset(level->map, 0, sizeof(level->map));

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("ERROR: Couldn't open map file: %s. Loading empty level.\n", filename);
        return;
    }

    char line[1024]; 
    int row = 0;

    // Read the file line by line until the end of the file or the maximum number of rows is reached
    while (fgets(line, sizeof(line), file) && row < LEVEL_ROWS) {
        int col = 0;
        char *token = strtok(line, ",");
        
        while (token != NULL && col < LEVEL_COLS) {
            // Convert the extracted string token into an integer tile value
            int tileValue = atoi(token);
            
            // If the tile is a spawn point, set the player's starting coordinates and leave the map tile empty
            if (tileValue == TILE_SPAWN) {
                level->spawnX = col * (float)TILE_SIZE;
                level->spawnY = row * (float)TILE_SIZE;
                level->map[row][col] = TILE_EMPTY;
            } else {
                level->map[row][col] = tileValue; 
            }
            // Assign the parsed tile value to the corresponding map grid position
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }
    fclose(file);
}

void initLevel(Level *level, int levelNumber) {
    char filename[32];

    sprintf(filename, "resource/maps/level%d.csv", levelNumber);
    printf("Loading level: %s\n", filename);
    loadLevelFromFile(level, filename);
}

void renderLevel(Level *level, SDL_Renderer *renderer) {
    for (int y = 0; y < LEVEL_ROWS; y++) {
        for (int x = 0; x < LEVEL_COLS; x++) {
            int tile = level->map[y][x];

            if (tile == TILE_EMPTY) continue;
            
            // Calculate the exact screen coordinates and dimensions for the current tile bounding box
            SDL_Rect tileRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            
            switch (tile) {
                case TILE_WALL:
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    SDL_RenderFillRect(renderer, &tileRect);
                    break;   
                case TILE_DANGER:
                    SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
                    SDL_RenderFillRect(renderer, &tileRect);
                    break;
                case TILE_FINISH:
                    SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
                    SDL_RenderFillRect(renderer, &tileRect);
                    break;
                case TILE_COIN:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                    SDL_Rect coinRect = { x * TILE_SIZE + 8, y * TILE_SIZE + 8, TILE_SIZE - 16, TILE_SIZE - 16 };
                    SDL_RenderFillRect(renderer, &coinRect);
                    break; 
                default:
                    break;
            }
        }
    }
}
