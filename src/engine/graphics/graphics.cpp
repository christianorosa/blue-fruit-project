#include "graphics.h"



namespace Graphics
{
    // ============================================================
    // Graphics state
    // ============================================================

    static HWND s_hwnd = nullptr;
    static HDC s_hdc = nullptr;

    static HDC s_backBufferDC = nullptr;
    static HBITMAP s_backBufferBitmap = nullptr;

    static int s_width = 800;
    static int s_height = 600;


    // ============================================================
    // Init
    // ============================================================

    bool Init(HWND hwnd)
    {
        if (hwnd == nullptr)
            return false;


        s_hwnd = hwnd;

        s_hdc = GetDC(hwnd);

        if (s_hdc == nullptr)
            return false;


        RECT rect{};

        GetClientRect(
            hwnd,
            &rect
        );


        s_width =
            rect.right - rect.left;

        s_height =
            rect.bottom - rect.top;


        if (s_width <= 0)
            s_width = 800;

        if (s_height <= 0)
            s_height = 600;


        // --------------------------------------------------------
        // Crear back buffer
        // --------------------------------------------------------

        s_backBufferDC =
            CreateCompatibleDC(s_hdc);

        if (s_backBufferDC == nullptr)
        {
            ReleaseDC(
                s_hwnd,
                s_hdc
            );

            s_hdc = nullptr;

            return false;
        }


        s_backBufferBitmap =
            CreateCompatibleBitmap(
                s_hdc,
                s_width,
                s_height
            );


        if (s_backBufferBitmap == nullptr)
        {
            DeleteDC(
                s_backBufferDC
            );

            s_backBufferDC = nullptr;


            ReleaseDC(
                s_hwnd,
                s_hdc
            );

            s_hdc = nullptr;

            return false;
        }


        SelectObject(
            s_backBufferDC,
            s_backBufferBitmap
        );


        return true;
    }


    // ============================================================
    // Resize
    // ============================================================

    void Resize(
        int width,
        int height)
    {
        if (width <= 0 ||
            height <= 0)
        {
            return;
        }


        s_width = width;
        s_height = height;


        if (s_backBufferBitmap)
        {
            DeleteObject(
                s_backBufferBitmap
            );

            s_backBufferBitmap = nullptr;
        }


        if (s_backBufferDC == nullptr ||
            s_hdc == nullptr)
        {
            return;
        }


        s_backBufferBitmap =
            CreateCompatibleBitmap(
                s_hdc,
                s_width,
                s_height
            );


        if (s_backBufferBitmap)
        {
            SelectObject(
                s_backBufferDC,
                s_backBufferBitmap
            );
        }
    }


    // ============================================================
    // Clear
    // ============================================================

    void Clear(
        const Color& color)
    {
        if (s_backBufferDC == nullptr)
            return;


        HBRUSH brush =
            CreateSolidBrush(
                RGB(
                    color.r,
                    color.g,
                    color.b
                )
            );


        RECT rect =
        {
            0,
            0,
            s_width,
            s_height
        };


        FillRect(
            s_backBufferDC,
            &rect,
            brush
        );


        DeleteObject(
            brush
        );
    }


    // ============================================================
    // DrawRect
    // ============================================================

    void DrawRect(
        int x,
        int y,
        int width,
        int height,
        const Color& color)
    {
        if (s_backBufferDC == nullptr)
            return;


        RECT rect =
        {
            x,
            y,
            x + width,
            y + height
        };


        HBRUSH brush =
            CreateSolidBrush(
                RGB(
                    color.r,
                    color.g,
                    color.b
                )
            );


        FillRect(
            s_backBufferDC,
            &rect,
            brush
        );


        DeleteObject(
            brush
        );
    }


    // ============================================================
    // DrawLine
    // ============================================================

    void DrawLine(
        int x1,
        int y1,
        int x2,
        int y2,
        const Color& color)
    {
        if (s_backBufferDC == nullptr)
            return;


        HPEN pen =
            CreatePen(
                PS_SOLID,
                1,
                RGB(
                    color.r,
                    color.g,
                    color.b
                )
            );


        HPEN oldPen =
            static_cast<HPEN>(
                SelectObject(
                    s_backBufferDC,
                    pen
                )
                );


        MoveToEx(
            s_backBufferDC,
            x1,
            y1,
            nullptr
        );


        LineTo(
            s_backBufferDC,
            x2,
            y2
        );


        SelectObject(
            s_backBufferDC,
            oldPen
        );


        DeleteObject(
            pen
        );
    }


    // ============================================================
    // DrawCircle
    // ============================================================

    void DrawCircle(
        int x,
        int y,
        int radius,
        const Color& color)
    {
        if (s_backBufferDC == nullptr)
            return;


        if (radius <= 0)
            return;


        HBRUSH brush =
            CreateSolidBrush(
                RGB(
                    color.r,
                    color.g,
                    color.b
                )
            );


        HBRUSH oldBrush =
            static_cast<HBRUSH>(
                SelectObject(
                    s_backBufferDC,
                    brush
                )
                );


        Ellipse(
            s_backBufferDC,

            x - radius,
            y - radius,

            x + radius,
            y + radius
        );


        SelectObject(
            s_backBufferDC,
            oldBrush
        );


        DeleteObject(
            brush
        );
    }


    // ============================================================
    // ProjectPoint
    //
    // World space -> screen space
    // ============================================================

    bool ProjectPoint(
        const bfp::Vector3D& point,
        int& screenX,
        int& screenY)
    {
        // --------------------------------------------------------
        // Punto detrás de la cámara
        // --------------------------------------------------------

        if (point.z <= 0.0f)
            return false;


        constexpr float focalLength =
            400.0f;


        // --------------------------------------------------------
        // Perspective projection
        // --------------------------------------------------------

        screenX =
            static_cast<int>(
                (point.x / point.z) *
                focalLength +
                static_cast<float>(s_width) * 0.5f
                );


        screenY =
            static_cast<int>(
                -(point.y / point.z) *
                focalLength +
                static_cast<float>(s_height) * 0.5f
                );


        return true;
    }


    // ============================================================
    // Project
    // ============================================================

    bfp::Vector2D Project(
        const bfp::Vector3D& position)
    {
        constexpr float focalLength =
            400.0f;


        if (position.z <= 0.01f)
        {
            return
            {
                0.0f,
                0.0f
            };
        }


        const float x =
            (position.x * focalLength) /
            position.z;


        const float y =
            (position.y * focalLength) /
            position.z;


        return
        {
            static_cast<float>(s_width) * 0.5f + x,
            static_cast<float>(s_height) * 0.5f - y
        };
    }


    
    


    // ============================================================
    // DrawCube
    //
    // Matrix-based version.
    // ============================================================

    void DrawCube(
        const bfp::Matrix4x4& transform,
        const Color& color)
    {
        bfp::Vector3D vertices[8] =
        {
            bfp::Vector3D(-0.5f, -0.5f, -0.5f),
            bfp::Vector3D(0.5f, -0.5f, -0.5f),
            bfp::Vector3D(0.5f,  0.5f, -0.5f),
            bfp::Vector3D(-0.5f,  0.5f, -0.5f),

            bfp::Vector3D(-0.5f, -0.5f,  0.5f),
            bfp::Vector3D(0.5f, -0.5f,  0.5f),
            bfp::Vector3D(0.5f,  0.5f,  0.5f),
            bfp::Vector3D(-0.5f,  0.5f,  0.5f)
        };

        int sx[8];
        int sy[8];

        for (int i = 0; i < 8; i++)
        {
            bfp::Vector3D world =
                transform.VectorMatrixMultiply(vertices[i]);

            if (!ProjectPoint(
                world,
                sx[i],
                sy[i]))
            {
                return;
            }
        }

        DrawLine(sx[0], sy[0], sx[1], sy[1], color);
        DrawLine(sx[1], sy[1], sx[2], sy[2], color);
        DrawLine(sx[2], sy[2], sx[3], sy[3], color);
        DrawLine(sx[3], sy[3], sx[0], sy[0], color);

        DrawLine(sx[4], sy[4], sx[5], sy[5], color);
        DrawLine(sx[5], sy[5], sx[6], sy[6], color);
        DrawLine(sx[6], sy[6], sx[7], sy[7], color);
        DrawLine(sx[7], sy[7], sx[4], sy[4], color);

        DrawLine(sx[0], sy[0], sx[4], sy[4], color);
        DrawLine(sx[1], sy[1], sx[5], sy[5], color);
        DrawLine(sx[2], sy[2], sx[6], sy[6], color);
        DrawLine(sx[3], sy[3], sx[7], sy[7], color);
    }


    
    // ============================================================
    // Display
    // ============================================================

    void Display()
    {
        if (s_hdc == nullptr ||
            s_backBufferDC == nullptr)
        {
            return;
        }


        BitBlt(
            s_hdc,

            0,
            0,

            s_width,
            s_height,

            s_backBufferDC,

            0,
            0,

            SRCCOPY
        );
    }


    // ============================================================
    // Shutdown
    // ============================================================

    void Shutdown()
    {
        if (s_backBufferBitmap)
        {
            DeleteObject(
                s_backBufferBitmap
            );

            s_backBufferBitmap = nullptr;
        }


        if (s_backBufferDC)
        {
            DeleteDC(
                s_backBufferDC
            );

            s_backBufferDC = nullptr;
        }


        if (s_hdc)
        {
            ReleaseDC(
                s_hwnd,
                s_hdc
            );

            s_hdc = nullptr;
        }


        s_hwnd = nullptr;


        s_width = 800;
        s_height = 600;
    }
}