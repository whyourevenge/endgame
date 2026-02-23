#include "level.h"

void loadLevelFromFile(Level *level, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Помилка: Не вдалося відкрити файл %s\n", filename);
        return;
    }

    char line[1024]; // Буфер для одного рядка файлу
    int row = 0;

    while (fgets(line, sizeof(line), file) && row < LEVEL_ROWS) {
        int col = 0;
        // Розбиваємо рядок на частини за допомогою коми
        char *token = strtok(line, ",");
        
        while (token != NULL && col < LEVEL_COLS) {
            level->map[row][col] = atoi(token); // Перетворюємо текст у число
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
    if (levelNumber == 1) {
        level->spawnX = 4 * TILE_SIZE;
        level->spawnY = 17 * TILE_SIZE;
    } 
    else if (levelNumber == 2) {
        level->spawnX = 7 * TILE_SIZE;
        level->spawnY = 6 * TILE_SIZE;
    }
    else if (levelNumber == 3) {
        level->spawnX = 2 * TILE_SIZE;
        level->spawnY = 4 * TILE_SIZE;
    }
    loadLevelFromFile(level, filename);
}

void renderLevel(Level *level, SDL_Renderer *renderer) {
    for (int y = 0; y < LEVEL_ROWS; y++) {
        for (int x = 0; x < LEVEL_COLS; x++) {
            SDL_Rect tileRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            
            if (level->map[y][x] == 1) { 
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderFillRect(renderer, &tileRect);
            } 
            else if (level->map[y][x] == 2) { 
                SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
                SDL_RenderFillRect(renderer, &tileRect);
            }
            else if (level->map[y][x] == 3) { 
                SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
                SDL_RenderFillRect(renderer, &tileRect);
            }
            else if (level->map[y][x] == 4) { 
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Жовтий для монеток
                SDL_Rect coinRect = { x * TILE_SIZE + 8, y * TILE_SIZE + 8, TILE_SIZE - 16, TILE_SIZE - 16 };
                SDL_RenderFillRect(renderer, &coinRect);
            }
        }
    }
}