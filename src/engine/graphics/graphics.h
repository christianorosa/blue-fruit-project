#pragma once
#include <windows.h>
#include "../../../src/engine/maths/include/Vector2D.h"
#include "../../../src/engine/maths/include/Vector3D.h"
#include "../../../src/engine/maths/include/Matrix.h"
#include "../../../src/engine/maths/include/Transform.h"




namespace Graphics
{

    

    struct Color
    {
        unsigned char r, g, b;

        Color(unsigned char r, unsigned char g, unsigned char b)
            : r(r), g(g), b(b) {
        }
    };

    static int g_Width = 800;
    static int g_Height = 600;


    bool Init(HWND hwnd);
    void Resize(int width, int height);
    void Clear(const Color& color);

    void DrawRect(int x, int y, int width, int height,
        const Color& color);

    void DrawLine(int x1, int y1, int x2, int y2,
        const Color& color);

    void DrawCircle(int x, int y, int radius,
        const Color& color);


    
    bool ProjectPoint(
        const bfp::Vector3D& point,
        int& screenX,
        int& screenY
    );
    
    bfp::Vector2D Project(const bfp::Vector3D& position);
	

    void DrawCube(
        const bfp::Matrix4x4& transform,
        const Color& color
    );



    void Display();
    void Shutdown();
}
