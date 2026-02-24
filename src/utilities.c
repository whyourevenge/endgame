#include "../inc/utilities.h"

void resetGame(App *app, Level *level, Player *player) {
    app->currentLevel = 1;
    app->deathCount = 0;
    app->gameStartTime = SDL_GetTicks();
    player->coins = 0;

    initLevel(level, app->currentLevel);
    initPlayer(player, level->spawnX, level->spawnY);
    printf("Game has been reset to Level 1.\n");
}