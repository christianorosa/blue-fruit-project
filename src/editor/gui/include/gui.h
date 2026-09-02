#pragma once
#include "../../../engine/input/include/input.h"
#include "guiContext.h"

namespace GUI
{
    Context& GetContext();

    void Init(void* nativeHandle);
    void BeginFrame();
    void EndFrame();

    void BeginWindow(const char* title);
    void EndWindow();

    bool Button(const char* label);
}
