#include "../include/core.h"
#include "../../window/include/thewindow.h"
#include "../../time/include/time.h"
#include "../../graphics/graphics.h"
#include "../../game/include/game.h"

namespace Core
{
    static bool g_Running = true;

    HINSTANCE* inst = nullptr;

    void Init(HINSTANCE* instance)
    {
        if (instance == nullptr || *instance == nullptr)
        {
            static HINSTANCE defaultInst = GetModuleHandle(nullptr);
            instance = &defaultInst;
        }

        inst = instance;

        if (!Window::Create(*inst, 800, 600,
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
        while (g_Running)
        {
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
        }
    }

    void Shutdown()
    {
        Graphics::Shutdown();
        Window::Destroy();
    }
}
