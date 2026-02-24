#include "../../inc/gui.h"

bool GUI_IsMouseInside(int mouseX, int mouseY, SDL_Rect rect) {
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

bool GUI_IsButtonClicked(SDL_Rect rect, int mx, int my, bool isLeftClicked, bool mouseReleased) {
    return (isLeftClicked && mouseReleased && GUI_IsMouseInside(mx, my, rect));
}
