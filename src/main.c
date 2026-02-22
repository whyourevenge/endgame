#include "endgame.h"
#include "player.h"
#include "level.h"
#include "menu.h"

bool initApp(App *app) {

// Initialization SDL, create window and renderer 

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initialization SDL: %s\n", SDL_GetError());
        return false;
    }

    app->window = SDL_CreateWindow(
        "Endgame: Platformer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0
    );

    if (!app->window) {
        printf("Error create window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!app->renderer) {
        printf("Error create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        return false;
    }
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Ошибка инициализации SDL_mixer: %s\n", Mix_GetError());
        // Мы не возвращаем false, чтобы игра работала даже если нет колонок
    } else {
        // Загружаем нашу музыку
        app->menuMusic = Mix_LoadMUS("resource/audio/main-menu-music.ogg");
        if (!app->menuMusic) {
            printf("Не удалось загрузить музыку: %s\n", Mix_GetError());
        } else {
            // Включаем музыку! 
            // -1 означает "повторять бесконечно" (зациклить)
            Mix_VolumeMusic(8);
            Mix_PlayMusic(app->menuMusic, -1);
        }

        app->winSound = Mix_LoadWAV("resource/audio/victory.wav");
        Mix_VolumeChunk(app->winSound, 24);
        if (!app->winSound) {
            printf("Не удалось загрузить звук победы: %s\n", Mix_GetError());
        }
    }

    app->victoryBg = IMG_LoadTexture(app->renderer, "resource/images/victory_bg.png");
    if (!app->victoryBg) {
        printf("Не удалось загрузить фон победы: %s\n", IMG_GetError());
    }

    if (TTF_Init() == -1) {
        printf("Помилка ініціалізації SDL_ttf: %s\n", TTF_GetError());
        return false;
    }
    // ВАЖЛИВО: Тобі треба буде закинути будь-який файл шрифту (.ttf) у папку resource!
    app->font = TTF_OpenFont("resource/Jersey10-Regular.ttf", 64); // 48 - це розмір шрифту
    if (!app->font) {
        printf("Не вдалося завантажити шрифт: %s\n", TTF_GetError());
    }

    app->isRunning = true;
    return true;
}

// Event processing
void handleEvents(App *app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            app->isRunning = false;
        }
        
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                if (app->state == STATE_PLAY) {
                    app->state = STATE_PAUSE;
                    app->mouseReleased = false; 
                } 
                else if (app->state == STATE_PAUSE) {
                    app->state = STATE_PLAY;
                }
                else if (app->state == STATE_SETTINGS) {
                    app->state = STATE_PAUSE;
                }
                else if (app->state == STATE_MENU) {
                    app->isRunning = false;
                }
            }
        }
    }
}

// Freeing resources
void cleanupApp(App *app) {
    if (app->victoryBg) {
        SDL_DestroyTexture(app->victoryBg);
    }

    if (app->menuMusic) {
        Mix_FreeMusic(app->menuMusic);
    }
    if (app->winSound) {
        Mix_FreeChunk(app->winSound);
    }
    Mix_CloseAudio();

    if (app->font) TTF_CloseFont(app->font);
    TTF_Quit();

    if (app->renderer) {
        SDL_DestroyRenderer(app->renderer);
    }
    if (app->window) {
        SDL_DestroyWindow(app->window);
    }
    SDL_Quit();
    printf("Game closed successfully. Resources freed.\n");
}

void resetGame(App *app, Level *level, Player *player) {
    app->currentLevel = 1;

    app->deathCount = 0;
    app->levelStartTime = SDL_GetTicks();

    initLevel(level, app->currentLevel);

    initPlayer(player, level->spawnX, level->spawnY);

    printf("Game has been reset to Level 1.\n");
}

int main(void) {
    App app = {0};

    if (!initApp(&app)) {
        return 1;
    }

    app.state = STATE_MENU;
    app.currentLevel = 1;

    Player player = {0};
    Level level = {0};

    initLevel(&level, app.currentLevel);

    initPlayer(&player, level.spawnX, level.spawnY);

    // --- ПЕРМІННІ ДЛЯ ЛІМІТУ FPS (ХОТФИКС) ---
    const int FPS = 60;
    const int frameDelay = 1000 / FPS; // Скільки мілісекунд має тривати 1 кадр (~16 мс)
    Uint32 frameStart;
    int frameTime;


    while (app.isRunning) {
        frameStart = SDL_GetTicks();

        handleEvents(&app);

        // --- ЛОГІКА (UPDATE) ЗАЛЕЖНО ВІД СТАНУ ---
        switch (app.state) {
            case STATE_MENU:
                updateMenu(&app);
                break;
            case STATE_PLAY:
                updatePlayer(&player, &level, &app); // Передаємо &app
                break;
            case STATE_PAUSE:
                updatePauseMenu(&app);
                break;
            case STATE_GAMEOVER:
                updateGameOver(&app, &level,&player);
                if (app.gameOverAlpha < 150) {
                app.gameOverAlpha += 5;
                }
                break;
            case STATE_SETTINGS:
                // Поки що налаштувань немає, просто по кліку ESC вийдемо в меню
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE]) {
                    app.state = STATE_MENU;
                }
                break;
            case STATE_VICTORY:
                // Якщо натиснули ENTER або ESC - повертаємося в головне меню
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RETURN] || 
                    SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE]) {
                    resetGame(&app, &level, &player);
                    app.state = STATE_MENU;
                    Mix_PlayMusic(app.menuMusic, -1);
                }
                break;
        }

        // --- РЕНДЕР (DRAW) ЗАЛЕЖНО ВІД СТАНУ ---
        // Очищаємо екран (чорний за замовчуванням)
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);

        switch (app.state) {
            case STATE_MENU:
                renderMenu(&app);
                break;
            case STATE_PLAY:
                renderLevel(&level, app.renderer);
                renderPlayer(&player, app.renderer);
                break;
            case STATE_PAUSE:
                // 1. Рисуем игру (она будет "заморожена" на фоне)
                renderLevel(&level, app.renderer);
                renderPlayer(&player, app.renderer);

                // 2. Затемняем экран
                SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 150); // Полупрозрачный черный
                SDL_Rect overlay = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
                SDL_RenderFillRect(app.renderer, &overlay);

                // 3. Рисуем кнопки паузы
                renderPauseMenu(&app); 
                break;
            case STATE_GAMEOVER:
                renderLevel(&level, app.renderer);
                renderPlayer(&player, app.renderer);
                SDL_SetRenderDrawColor(app.renderer, 100, 0, 0, app.gameOverAlpha); 
                SDL_Rect fullScreen = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
                SDL_RenderFillRect(app.renderer, &fullScreen);
                if (app.gameOverAlpha >= 150) {
                    renderGameOver(&app);
                }
                break;
            case STATE_SETTINGS:
                // Малюємо синій екран для налаштувань
                SDL_SetRenderDrawColor(app.renderer, 0, 0, 100, 255);
                SDL_RenderClear(app.renderer);
                break;
            case STATE_VICTORY:
                // 1. РИСУЕМ ФОНОВУЮ КАРТИНКУ
                if (app.victoryBg != NULL) {
                    SDL_RenderCopy(app.renderer, app.victoryBg, NULL, NULL);
                } else {
                    // Запасной план, если картинка не загрузилась
                    SDL_SetRenderDrawColor(app.renderer, 255, 215, 0, 255);
                    SDL_RenderClear(app.renderer);
                }

                // 2. РИСУЕМ ТЕКСТЫ
                if (app.font != NULL) {
                    // --- ГЛАВНЫЙ ТЕКСТ (Время и Смерти) ---
                    char timeText[128];
                    sprintf(timeText, "VICTORY! Time: %.2f sec | Deaths: %d", app.finalTime, app.deathCount);

                    SDL_Color whiteColor = {255, 255, 255, 255}; // Белый текст лучше видно на картинках
                    SDL_Surface *surf1 = TTF_RenderUTF8_Blended(app.font, timeText, whiteColor);
                    SDL_Texture *tex1 = SDL_CreateTextureFromSurface(app.renderer, surf1);

                    // Центрируем и поднимаем чуть ВЫШЕ середины экрана
                    SDL_Rect rect1 = { 
                        (WINDOW_WIDTH - surf1->w) / 2, 
                        (WINDOW_HEIGHT / 2) - surf1->h, 
                        surf1->w, 
                        surf1->h 
                    };
                    SDL_RenderCopy(app.renderer, tex1, NULL, &rect1);

                    // --- ТЕКСТ-ИНСТРУКЦИЯ ---
                    const char *hintText = "Press [ENTER] to return to Main Menu";
                    SDL_Color grayColor = {200, 200, 200, 255}; // Слегка серый цвет для второстепенного текста
                    SDL_Surface *surf2 = TTF_RenderUTF8_Blended(app.font, hintText, grayColor);
                    SDL_Texture *tex2 = SDL_CreateTextureFromSurface(app.renderer, surf2);

                    // Центрируем и опускаем чуть НИЖЕ середины экрана
                    SDL_Rect rect2 = { 
                        (WINDOW_WIDTH - surf2->w) / 2, 
                        (WINDOW_HEIGHT / 2) + 20, 
                        surf2->w, 
                        surf2->h 
                    };
                    SDL_RenderCopy(app.renderer, tex2, NULL, &rect2);

                    // Очищаем память от поверхностей и текстур текста
                    SDL_FreeSurface(surf1);
                    SDL_DestroyTexture(tex1);
                    SDL_FreeSurface(surf2);
                    SDL_DestroyTexture(tex2);
                }
                break;
        }

        SDL_RenderPresent(app.renderer);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    cleanupApp(&app);
    return 0;
}
