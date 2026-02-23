#include "player.h"

void initPlayer(Player *p, float spawnX, float spawnY) {
    p->width = 32;
    p->height = 32;
    p->x = spawnX;
    p->y = spawnY;

    p->dx = 0.0f;
    p->dy = 0.0f;
    p->gravDir = GRAV_DOWN; 
    p->spawnTime = SDL_GetTicks();
}

int checkCollision(Player *p, Level *level) {
  
    int leftTile = p->x / TILE_SIZE;
    int rightTile = (p->x + p->width - 1) / TILE_SIZE;
    int topTile = p->y / TILE_SIZE;
    int bottomTile = (p->y + p->height - 1) / TILE_SIZE;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (x >= 0 && x < LEVEL_COLS && y >= 0 && y < LEVEL_ROWS) {
                if (level->map[y][x] == 1)  
                    return 1; 
            }
        }
    }
    return 0; 
}

void checkCoinCollision(App *app, Player *player, Level *level) {
 
    int centerX = (player->x + TILE_SIZE / 2) / TILE_SIZE;
    int centerY = (player->y + TILE_SIZE / 2) / TILE_SIZE;
    if (centerX >= 0 && centerX < LEVEL_COLS && centerY >= 0 && centerY < LEVEL_ROWS) {
        if (level->map[centerY][centerX] == 4) {
            level->map[centerY][centerX] = 0; 
            player->coins++;                 
            printf("Coins collected: %d\n", player->coins);
            if (app->coinSound) 
                Mix_PlayChannel(-1, app->coinSound, 0);
        }
    }
}

int checkWin(Player *p, Level *level) {
    int leftTile = p->x / TILE_SIZE;
    int rightTile = (p->x + p->width - 1) / TILE_SIZE;
    int topTile = p->y / TILE_SIZE;
    int bottomTile = (p->y + p->height - 1) / TILE_SIZE;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (x >= 0 && x < LEVEL_COLS && y >= 0 && y < LEVEL_ROWS) {
                if (level->map[y][x] == 3)  
                    return 1;
            }
        }
    }
    return 0;
}

int checkDeath(Player *p, Level *level) {

    if (p->x < 0 || p->x + p->width > LEVEL_COLS * TILE_SIZE ||
        p->y < 0 || p->y + p->height > LEVEL_ROWS * TILE_SIZE)
        return 1;
   
    int leftTile = p->x / TILE_SIZE;
    int rightTile = (p->x + p->width - 1) / TILE_SIZE;
    int topTile = p->y / TILE_SIZE;
    int bottomTile = (p->y + p->height - 1) / TILE_SIZE;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (x >= 0 && x < LEVEL_COLS && y >= 0 && y < LEVEL_ROWS) {
                if (level->map[y][x] == 2) 
                    return 1;
            }
        }
    }
    return 0;
}

void updatePlayer(Player *p, Level *level, App *app) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    float gravity = 0.6f;      
    float maxFallSpeed = 12.0f; 
    float walkSpeed = 6.0f;    

    Uint32 currentTime = SDL_GetTicks();
    Uint32 spawnDelay = 1000; 

    if (currentTime - p->spawnTime >= spawnDelay) {
        if (keys[SDL_SCANCODE_UP])    p->gravDir = GRAV_UP;
        if (keys[SDL_SCANCODE_DOWN])  p->gravDir = GRAV_DOWN;
        if (keys[SDL_SCANCODE_LEFT])  p->gravDir = GRAV_LEFT;
        if (keys[SDL_SCANCODE_RIGHT]) p->gravDir = GRAV_RIGHT;
    }
    if (p->gravDir == GRAV_DOWN || p->gravDir == GRAV_UP) {
        p->dx = 0.0f;
        if (keys[SDL_SCANCODE_A]) p->dx = -walkSpeed;
        if (keys[SDL_SCANCODE_D]) p->dx = walkSpeed;

        if (p->gravDir == GRAV_DOWN) {
            p->dy += gravity;
            if (p->dy > maxFallSpeed) p->dy = maxFallSpeed;
        } 
        else {
            p->dy -= gravity;
            if (p->dy < -maxFallSpeed) p->dy = -maxFallSpeed;
        }
    } 
    else {
        p->dy = 0.0f;
        if (keys[SDL_SCANCODE_W]) p->dy = -walkSpeed;
        if (keys[SDL_SCANCODE_S]) p->dy = walkSpeed;

        if (p->gravDir == GRAV_RIGHT) {
            p->dx += gravity;
            if (p->dx > maxFallSpeed) p->dx = maxFallSpeed;
        } 
        else {
            p->dx -= gravity;
            if (p->dx < -maxFallSpeed) p->dx = -maxFallSpeed;
        }
    }
    p->x += p->dx; 
    checkCoinCollision(app, p, level);
    if (checkCollision(p, level)) {
        if (p->dx > 0) {
            p->x = ((int)(p->x + p->width) / TILE_SIZE) * TILE_SIZE - p->width;
        } 
        else if (p->dx < 0)  
            p->x = ((int)p->x / TILE_SIZE + 1) * TILE_SIZE;
        
        p->dx = 0.0f; 
    }

    p->y += p->dy;
    if (checkCollision(p, level)) {
        if (p->dy > 0) 
            p->y = ((int)(p->y + p->height) / TILE_SIZE) * TILE_SIZE - p->height;
         
        else if (p->dy < 0) 
            p->y = ((int)p->y / TILE_SIZE + 1) * TILE_SIZE;
      
        p->dy = 0.0f;
    }

    if (checkDeath(p, level)) {
        app->deathCount++;
        Mix_HaltMusic(); 
        printf("[STATISTICS] Oops! Gravity won. Deaths per game: %d\n", app->deathCount);
        app->state = STATE_GAMEOVER;
        app->gameOverAlpha = 0;
    }
    if (checkWin(p, level)) {
    
        app->currentLevel++; 
        if (app->currentLevel > 3) {

            app->finalTime = (SDL_GetTicks() - app->gameStartTime) / 1000.0f;
            Mix_HaltMusic();
            printf("[FINAL] The game is fully completed!\n");
            printf("Total time: %.2f seconds\n", app->finalTime);
            
            if (app->winSound) 
                Mix_PlayChannel(-1, app->winSound, 0);

            app->state = STATE_VICTORY;
        } 
        else {
            
            printf("Level completed! Let`s move on to the next level %d\n", app->currentLevel);
            initLevel(level, app->currentLevel);
            initPlayer(p, level->spawnX, level->spawnY);  
            p->coinsAtLevelStart = p->coins;
            initPlayer(p, level->spawnX, level->spawnY); 
             
                 if (app->currentLevel == 2 && app->level2Music) 
                     Mix_PlayMusic(app->level2Music, -1);
            
                 else if (app->currentLevel == 3 && app->level3Music) 
                     Mix_PlayMusic(app->level3Music, -1);
            
        }
    }
}

void renderPlayer(Player *p, SDL_Renderer *renderer) {
    SDL_Rect rect = { (int)p->x, (int)p->y, p->width, p->height };
    SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
    SDL_RenderFillRect(renderer, &rect);
}
