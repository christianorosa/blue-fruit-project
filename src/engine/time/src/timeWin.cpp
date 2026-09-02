#include "../include/time.h"

namespace Time
{
    LARGE_INTEGER g_Frequency = {};
    LARGE_INTEGER g_LastCounter = {};
    double g_DeltaTime = 0.0;
    double g_Time = 0.0;

    void Init()
    {
        QueryPerformanceFrequency(&g_Frequency);
        QueryPerformanceCounter(&g_LastCounter);

        g_DeltaTime = 0.0;
        g_Time = 0.0;
    }

    void Update()
    {
        LARGE_INTEGER currentCounter;
        QueryPerformanceCounter(&currentCounter);

        const LONGLONG elapsed =
            currentCounter.QuadPart - g_LastCounter.QuadPart;

        g_DeltaTime =
            static_cast<double>(elapsed) /
            static_cast<double>(g_Frequency.QuadPart);

        g_Time += g_DeltaTime;
        g_LastCounter = currentCounter;
    }

    double DeltaTime()
    {
        return g_DeltaTime;
    }

    double TimeSinceStart()
    {
        return g_Time;
    }
}
