#pragma once

#include "../../data/include/scene.h"
#include "../../data/include/virtualFileSystem.h"
#include "../../data/include/archive.h"
#include <filesystem>

namespace Game
{
    extern Scene* scn;
    extern bfp::VirtualFileSystem* vfs;


    void Init();
    void Update(double deltaTime);
    void Render();
    void Shutdown();
}
