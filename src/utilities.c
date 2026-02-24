#include "../inc/utilities.h"

void resetGame(Game *game, Level *level, Player *player) {
    game->currentLevel = 1;
    game->deathCount = 0;
    game->gameStartTime = SDL_GetTicks();
    player->coins = 0;

    initLevel(level, game->currentLevel);
    initPlayer(player, level->spawnX, level->spawnY);
    printf("Game has been reset to Level 1.\n");
}