#include "../include/core.h"
#include "../../window/include/thewindow.h"
#include "../../time/include/time.h"
#include "../../graphics/graphics.h"
#include "../../game/include/game.h"

#include <windows.h>

namespace Core
{
    static bool g_Running = true;

    static int g_TargetFPS = 0;
    static double g_TargetFrameTime = 0.0;

    HINSTANCE* inst = nullptr;

    void Init(HINSTANCE* instance)
    {
        if (instance == nullptr || *instance == nullptr)
        {
            static HINSTANCE defaultInst = GetModuleHandle(nullptr);
            instance = &defaultInst;
        }

        inst = instance;

		//FPS limiter
        SetTargetFPS(144);


        if (!Window::Create(
            *inst,
            800,
            600,
            L"Destruction system by objects and time"))
        {
            g_Running = false;
            return;
        }

        Time::Init();

        if (!Graphics::Init(Window::GetHandle()))
        {
            g_Running = false;
            return;
        }

        g_Running = true;
    }

    void Run()
    {
        LARGE_INTEGER frequency;
        LARGE_INTEGER frameStart;
        LARGE_INTEGER frameEnd;

        QueryPerformanceFrequency(&frequency);

        while (g_Running)
        {
            QueryPerformanceCounter(&frameStart);

            Window::PollEvents();

            if (Window::ShouldClose())
            {
                g_Running = false;
                break;
            }

            Time::Update();

            const double deltaTime = Time::DeltaTime();

            Game::Update(deltaTime);
            Game::Render();

            Graphics::Display();

            // FPS limiter
            if (g_TargetFPS > 0)
            {
                QueryPerformanceCounter(&frameEnd);

                const double frameTime =
                    static_cast<double>(
                        frameEnd.QuadPart - frameStart.QuadPart
                        ) /
                    static_cast<double>(frequency.QuadPart);

                const double remaining =
                    g_TargetFrameTime - frameTime;

                if (remaining > 0.0)
                {
                    // Dormimos la mayor parte del tiempo restante.
                    // Dejamos un pequeño margen para evitar pasarnos.
                    if (remaining > 0.002)
                    {
                        DWORD sleepTime =
                            static_cast<DWORD>(
                                (remaining - 0.001) * 1000.0
                                );

                        if (sleepTime > 0)
                            Sleep(sleepTime);
                    }

                    // Espera de alta precisión para completar el frame.
                    do
                    {
                        QueryPerformanceCounter(&frameEnd);

                        const double elapsed =
                            static_cast<double>(
                                frameEnd.QuadPart - frameStart.QuadPart
                                ) /
                            static_cast<double>(frequency.QuadPart);

                        if (elapsed >= g_TargetFrameTime)
                            break;

                        YieldProcessor();

                    } while (true);
                }
            }
        }
    }

    void Shutdown()
    {
        Graphics::Shutdown();
        Window::Destroy();
    }

    void SetTargetFPS(int fps)
    {
        if (fps < 0)
            fps = 0;

        g_TargetFPS = fps;

        if (g_TargetFPS > 0)
        {
            g_TargetFrameTime =
                1.0 / static_cast<double>(g_TargetFPS);
        }
        else
        {
            g_TargetFrameTime = 0.0;
        }
    }

    int GetTargetFPS()
    {
        return g_TargetFPS;
    }
}