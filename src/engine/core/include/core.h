#pragma once

#include <windows.h>

namespace Core
{
    extern HINSTANCE* inst;

    void Init(HINSTANCE* instance);
    void Run();
    void Shutdown();


    void SetTargetFPS(int fps);
    int GetTargetFPS();

}
