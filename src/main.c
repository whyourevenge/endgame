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

    if (TTF_Init() == -1) {
        printf("Помилка ініціалізації SDL_ttf: %s\n", TTF_GetError());
        return false;
    }
    // ВАЖЛИВО: Тобі треба буде закинути будь-який файл шрифту (.ttf) у папку resource!
    app->font = TTF_OpenFont("resource/Jersey10-Regular.ttf", 48); // 48 - це розмір шрифту
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
        
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            app->isRunning = false;
        }
    }
}

// Freeing resources
void cleanupApp(App *app) {
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

                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE]) {
                    app.state = STATE_MENU;
                    
                    // --- ВКЛЮЧАЕМ МУЗЫКУ ОБРАТНО ---
                    // Здесь app - это обычная структура, поэтому используем точку (.)
                    Mix_PlayMusic(app.menuMusic, -1); 
                }
                break;
            case STATE_GAMEOVER:
                updateGameOver(&app, &level,&player);
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
                    app.state = STATE_MENU;
                    app.currentLevel = 1; // Скидаємо прогрес, щоб можна було почати заново
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
            case STATE_GAMEOVER:
                renderGameOver(&app);
                break;
            case STATE_SETTINGS:
                // Малюємо синій екран для налаштувань
                SDL_SetRenderDrawColor(app.renderer, 0, 0, 100, 255);
                SDL_RenderClear(app.renderer);
                break;
            case STATE_VICTORY:
                // Золотий фон
                SDL_SetRenderDrawColor(app.renderer, 255, 215, 0, 255);
                SDL_RenderClear(app.renderer);

                if (app.font != NULL) {
                    char timeText[64];
                    // Формуємо рядок тексту з нашим часом
                    sprintf(timeText, "VICTORY! Your time: %.2f sec", app.finalTime);

                    SDL_Color textColor = {0, 0, 0, 255}; // Чорний текст
                    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(app.font, timeText, textColor);
                    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(app.renderer, textSurface);

                    // Центруємо текст на екрані
                    SDL_Rect textRect = { 
                        (WINDOW_WIDTH - textSurface->w) / 2, 
                        (WINDOW_HEIGHT - textSurface->h) / 2, 
                        textSurface->w, 
                        textSurface->h 
                    };

                    SDL_RenderCopy(app.renderer, textTexture, NULL, &textRect);

                    // Очищаємо пам'ять кадру
                    SDL_FreeSurface(textSurface);
                    SDL_DestroyTexture(textTexture);
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
