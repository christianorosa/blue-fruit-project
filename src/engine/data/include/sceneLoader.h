#pragma once

#include "scene.h"
#include "virtualFileSystem.h"

namespace sceneLoader
{
    bool LoadScene(
        const char* filename,
        Scene& scene,
        bfp::VirtualFileSystem& vfs
    );
}