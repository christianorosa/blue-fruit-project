#pragma once
#include <windows.h>

namespace GUI
{
    struct Context
    {
        int mouseX = 0;
        int mouseY = 0;
        bool mousePressed = false;

        int hotItem = 0;
        int activeItem = 0;

        int cursorX = 0;
        int cursorY = 0;

        int windowX = 0;
        int windowY = 0;
        int windowW = 0;
        int windowH = 0;

        HDC hdc = nullptr;
    };

    Context& GetContext();
}
