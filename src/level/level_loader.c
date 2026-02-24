#include "../../inc/level.h"

void loadLevelFromFile(Level *level, const char *filename) {
    memset(level->map, 0, sizeof(level->map));

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("ERROR: Couldn't open map file: %s. Loading empty level.\n", filename);
        return;
    }

    char line[1024]; 
    int row = 0;

    while (fgets(line, sizeof(line), file) && row < LEVEL_ROWS) {
        int col = 0;
        char *token = strtok(line, ",");
        
        while (token != NULL && col < LEVEL_COLS) {
            int tileValue = atoi(token);
            
            if (tileValue == TILE_SPAWN) {
                level->spawnX = col * (float)TILE_SIZE;
                level->spawnY = row * (float)TILE_SIZE;
                level->map[row][col] = TILE_EMPTY;
            } else {
                level->map[row][col] = tileValue; 
            }
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