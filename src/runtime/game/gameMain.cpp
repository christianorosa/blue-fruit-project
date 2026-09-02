#include "../../engine/data/include/scene.h"
#include "../../engine/data/include/sceneLoader.h"
#include "../../engine/core/include/core.h"
#include "../../engine/game/include/game.h"
#include <filesystem>
#include <windows.h>

int main()
{
    HINSTANCE currentInstance = GetModuleHandle(NULL);
    Core::inst = &currentInstance;

    Core::Init(Core::inst);

    Game::Init();


    std::printf(
        "Working directory: %s\n",
        std::filesystem::current_path().string().c_str()
    );

    if (Game::scn != nullptr)
    {
        sceneLoader::LoadScene(
            "levels/level01.json",
            *Game::scn,
            *Game::vfs
        );
    }

    Core::Run();

    Game::Shutdown();
    Core::Shutdown();

    return 0;
}