#pragma once
#include <windows.h>

namespace Window
{
    bool Create(HINSTANCE instance, int width, int height,
        const wchar_t* title);

    void PollEvents();
    bool ShouldClose();
    void Destroy();

    HWND GetHandle();
}
