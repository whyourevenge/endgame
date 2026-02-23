#include "endgame.h"

void toggle_sound(App *app) {
    if (app->volume > 0)
        app->volume = 0;
    else
        app->volume = 64;
    Mix_VolumeMusic(app->volume);
}
