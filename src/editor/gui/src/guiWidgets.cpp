#include "../include/guiWidgets.h"
#include "../include/gui.h"
#include <windows.h>

namespace GUI
{
    static int g_NextID = 1;

    bool Button(const char* label)
    {
        Context& ctx = GetContext();

        int id = g_NextID++;

        int x = ctx.cursorX;
        int y = ctx.cursorY;
        int w = 120;
        int h = 30;

        // Detectar hover
        bool hovered =
            ctx.mouseX >= x &&
            ctx.mouseX <= x + w &&
            ctx.mouseY >= y &&
            ctx.mouseY <= y + h;

        if (hovered)
            ctx.hotItem = id;

        // Detectar click
        if (hovered && ctx.mousePressed && ctx.activeItem == 0)
            ctx.activeItem = id;

        bool clicked = false;

        if (!ctx.mousePressed &&
            ctx.hotItem == id &&
            ctx.activeItem == id)
        {
            clicked = true;
        }

        // Dibujado
        if (ctx.hdc)
        {
            if (ctx.hotItem == id)
            {
                HBRUSH brush = CreateSolidBrush(RGB(80, 80, 80));
                RECT r = { x, y, x + w, y + h };
                FillRect(ctx.hdc, &r, brush);
                DeleteObject(brush);
            }
            else
            {
                Rectangle(ctx.hdc, x, y, x + w, y + h);
            }

            TextOutA(
                ctx.hdc,
                x + 10,
                y + 8,
                label,
                lstrlenA(label)
            );
        }

        // Avanzar layout vertical
        ctx.cursorY += h + 10;

        return clicked;
    }
}