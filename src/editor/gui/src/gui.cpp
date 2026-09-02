#include "../include/gui.h"

namespace GUI
{
    static Context g_ctx;

    Context& GetContext()
    {
        return g_ctx;
    }

    void BeginFrame()
    {
        g_ctx.cursorX = 20;
        g_ctx.cursorY = 20;
    }

    void BeginWindow(const char* title)
    {
        Context& ctx = GetContext();

        ctx.windowX = 10;
        ctx.windowY = 10;
        ctx.windowW = 300;
        ctx.windowH = 400;
    }

    void EndWindow() 
    {
    }

    
}