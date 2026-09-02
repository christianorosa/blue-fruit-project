#include "../include/thewindow.h"
#include "../../graphics/graphics.h"

namespace Window
{
    static HWND s_hwnd = nullptr;
    static bool s_running = true;

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_SIZE:
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);

            Graphics::Resize(width, height);
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_CLOSE:
            s_running = false;
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    bool Create(HINSTANCE instance, int width, int height,
        const wchar_t* title)
    {
        WNDCLASSW wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = instance;
        wc.lpszClassName = L"MyWindowClass";

        RegisterClassW(&wc);

        s_hwnd = CreateWindowExW(
            0,
            wc.lpszClassName,
            title,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
            nullptr,
            nullptr,
            instance,
            nullptr
        );

        if (!s_hwnd)
            return false;

        ShowWindow(s_hwnd, SW_SHOW);

        return true;
    }

    void PollEvents()
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool ShouldClose()
    {
        return !s_running;
    }

    void Destroy()
    {
        if (s_hwnd)
        {
            DestroyWindow(s_hwnd);
            s_hwnd = nullptr;
        }
    }

    HWND GetHandle()
    {
        return s_hwnd;
    }
}
