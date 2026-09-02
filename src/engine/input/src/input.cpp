#include "../include/input.h"
#include <windows.h>

namespace Input
{
    static int s_mouseX = 0;
    static int s_mouseY = 0;
    static bool s_mousePressed = false;

    void Init()
    {
        s_mouseX = 0;
        s_mouseY = 0;
        s_mousePressed = false;
    }

    void Update()
    {
        POINT p;
        GetCursorPos(&p);

        s_mouseX = p.x;
        s_mouseY = p.y;

        s_mousePressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    }

    int GetMouseX()
    {
        return s_mouseX;
    }

    int GetMouseY()
    {
        return s_mouseY;
    }

    bool IsMouseButtonPressed()
    {
        return s_mousePressed;
    }
}