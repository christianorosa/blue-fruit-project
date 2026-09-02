#include "../include/gui.h"
#include <windows.h>

namespace GUI
{
    void Init(void* hwnd)
    {
        Context& ctx = GetContext();
        ctx.hdc = GetDC((HWND)hwnd);
    }
}