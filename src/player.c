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

// Допоміжна функція для колізії (AABB проти Тайл-мапи)
// Вона перевіряє, чи торкається гравець будь-якого блоку '1'
int checkCollision(Player *p, Level *level) {
    // Рахуємо краї гравця (в пікселях)
    // Віднімаємо 1 піксель від правого та нижнього краю. Чому?
    // Якщо гравець стоїть рівно на координаті 64 (початок 3-го тайлу), 
    // його ширина 32. 64 + 32 = 96 (це вже початок 4-го тайлу). 
    // Без -1 він би чіпляв сусідній порожній тайл і "застрягав" би в повітрі.
    int leftTile = p->x / TILE_SIZE;
    int rightTile = (p->x + p->width - 1) / TILE_SIZE;
    int topTile = p->y / TILE_SIZE;
    int bottomTile = (p->y + p->height - 1) / TILE_SIZE;

    // Проходимось по всіх тайлах, які зараз перекриває гравець
    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            // Захист від виходу за межі масиву (щоб гра не крашнулася)
            if (x >= 0 && x < LEVEL_COLS && y >= 0 && y < LEVEL_ROWS) {
                if (level->map[y][x] == 1) { // Якщо знайшли стіну
                    return 1; // Є колізія!
                }
            }
        }
    }
    return 0; // Колізій немає
}

int checkWin(Player *p, Level *level) {
    int leftTile = p->x / TILE_SIZE;
    int rightTile = (p->x + p->width - 1) / TILE_SIZE;
    int topTile = p->y / TILE_SIZE;
    int bottomTile = (p->y + p->height - 1) / TILE_SIZE;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (x >= 0 && x < LEVEL_COLS && y >= 0 && y < LEVEL_ROWS) {
                if (level->map[y][x] == 3) { // Фініш!
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Повертає 1, якщо гравець помер, і 0, якщо живий
int checkDeath(Player *p, Level *level) {
    // 1. Перевірка вильоту за межі екрану (у відкритий космос)
    if (p->x < 0 || p->x + p->width > LEVEL_COLS * TILE_SIZE ||
        p->y < 0 || p->y + p->height > LEVEL_ROWS * TILE_SIZE) {
        return 1;
    }

    // 2. Перевірка дотику до шипів (Тайл 2)
    int leftTile = p->x / TILE_SIZE;
    int rightTile = (p->x + p->width - 1) / TILE_SIZE;
    int topTile = p->y / TILE_SIZE;
    int bottomTile = (p->y + p->height - 1) / TILE_SIZE;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (x >= 0 && x < LEVEL_COLS && y >= 0 && y < LEVEL_ROWS) {
                if (level->map[y][x] == 2) { // Торкнулися шипа!
                    return 1;
                }
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
    Uint32 spawnDelay = 1000; // Задержка в миллисекундах (1000 = 1 секунда). Можешь поставить 500.

    // Разрешаем менять гравитацию ТОЛЬКО если прошла 1 секунда после старта уровня
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
        } else {
            p->dy -= gravity;
            if (p->dy < -maxFallSpeed) p->dy = -maxFallSpeed;
        }
    } else {
        p->dy = 0.0f;
        if (keys[SDL_SCANCODE_W]) p->dy = -walkSpeed;
        if (keys[SDL_SCANCODE_S]) p->dy = walkSpeed;

        if (p->gravDir == GRAV_RIGHT) {
            p->dx += gravity;
            if (p->dx > maxFallSpeed) p->dx = maxFallSpeed;
        } else {
            p->dx -= gravity;
            if (p->dx < -maxFallSpeed) p->dx = -maxFallSpeed;
        }
    }

    
    p->x += p->dx; 
    if (checkCollision(p, level)) {
        // Якщо врізалися - відштовхуємось назад
        if (p->dx > 0) { // Рухались вправо
            // Ставимо гравця впритул до лівого краю тайлу, в який врізалися
            p->x = ((int)(p->x + p->width) / TILE_SIZE) * TILE_SIZE - p->width;
        } else if (p->dx < 0) { // Рухались вліво
            // Ставимо впритул до правого краю тайлу
            p->x = ((int)p->x / TILE_SIZE + 1) * TILE_SIZE;
        }
        p->dx = 0.0f; // Зупиняємо швидкість по X
    }

    // --- 4. РУХ ПО Y ТА КОЛІЗІЯ ---
    p->y += p->dy;
    if (checkCollision(p, level)) {
        if (p->dy > 0) { // Падали вниз
            p->y = ((int)(p->y + p->height) / TILE_SIZE) * TILE_SIZE - p->height;
        } else if (p->dy < 0) { // Падали вгору (на стелю)
            p->y = ((int)p->y / TILE_SIZE + 1) * TILE_SIZE;
        }
        p->dy = 0.0f; // Зупиняємо швидкість по Y
    }

    // --- 5. ПЕРЕВІРКА СМЕРТІ ---
    if (checkDeath(p, level)) {
        app->deathCount++; // Плюсуємо смерть
        printf("[СТАТИСТИКА] Упс! Гравітація перемогла. Смертей за гру: %d\n", app->deathCount);
        
        app->state = STATE_GAMEOVER;
    }

    // --- 6. ПЕРЕВІРКА ПЕРЕМОГИ ---
    if (checkWin(p, level)) {
        app->currentLevel++; // Збільшуємо номер рівня
        
        // ПЕРЕВІРКА НА ФІНАЛ ГРИ
        if (app->currentLevel > 3) {
            // ЗБЕРІГАЄМО ЧАС У СТРУКТУРУ APP:
            app->finalTime = (SDL_GetTicks() - app->gameStartTime) / 1000.0f;
            
            printf("[ФІНАЛ] Гру пройдено повністю!\n");
            printf(" -> Загальний час: %.2f секунд\n", app->finalTime);
            
            if (app->winSound) {
                Mix_PlayChannel(-1, app->winSound, 0);
            }

            app->state = STATE_VICTORY;
        } else {
            // Інакше — просто завантажуємо наступний рівень 
            printf("Рівень пройдено! Переходимо на рівень %d\n", app->currentLevel);
            initLevel(level, app->currentLevel);
            initPlayer(p, level->spawnX, level->spawnY);      
            
            // ВАЖЛИВО: Ми більше НЕ скидаємо app->deathCount 
            // і НЕ оновлюємо таймер! Час продовжує йти.
        }
    }
}

void renderPlayer(Player *p, SDL_Renderer *renderer) {
    SDL_Rect rect = { (int)p->x, (int)p->y, p->width, p->height };
    SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
    SDL_RenderFillRect(renderer, &rect);
}
