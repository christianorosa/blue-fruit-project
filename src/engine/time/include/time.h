#pragma once

#include <windows.h>

namespace Time
{
    extern LARGE_INTEGER g_Frequency;
    extern LARGE_INTEGER g_LastCounter;
    extern double g_DeltaTime;
    extern double g_Time;

    void Init();
    void Update();

    double DeltaTime();
    double TimeSinceStart();
}