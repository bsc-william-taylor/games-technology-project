
#include <ShellScalingApi.h>
#include "HighPrecisionTimer.h"
#include "System.h"

OperatingSystem::OperatingSystem()
    : sceneManager(&windowManager), swapBuffers(true)
{
}

OperatingSystem::~OperatingSystem()
{
}

Gamepad * OperatingSystem::acquireGamepad()
{
    return sceneManager.getGamepad();
}

bool OperatingSystem::setupLibraries()
{
    auto SDL_Params{ SDL_INIT_EVERYTHING };
    auto setupSuccess{ true };

    if (SDL_Init(SDL_Params) != NULL) 
    {
        throw Error(Component::Game, "Couldnt init SDL");
    }

    if (TTF_Init() != NULL) 
    {
        throw Error(Component::Game, "Couldnt init TTF");
    }
    
    BASS_Init(-1, 44100, NULL, nullptr, nullptr);

    FreeImage_Initialise();

    srand(time(nullptr));

    return setupSuccess;
}

void OperatingSystem::releaseLibraries()
{
    FreeImage_DeInitialise();
    BASS_Free();
    TTF_Quit();
    SDL_Quit();
}

void OperatingSystem::executeProgram()
{
    HighPrecisionTimer frameTimer;
    SDL_Event systemEvent;
    auto gpu{ GPU(true) };
    auto run{ SDL_TRUE };

    if (gpu.has(OPENGL, 3.3))
    {
        sceneManager.createScenes();

        while (run)
        {
            frameTimer.start();

            while (SDL_PollEvent(&systemEvent))
            {
                if (systemEvent.type == SDL_QUIT)
                {
                    run = SDL_FALSE;
                    break;
                }
                
                sceneManager.handeEventScene(systemEvent);
            }
            
            sceneManager.updateScene();
            sceneManager.renderScene();

            if (swapBuffers) 
            {
                windowManager.swapBuffers();
            }

            if (!windowManager.vsyncEnabled())
            {
                while (frameTimer.elapsed(Nanoseconds) <= static_cast<float>(1.0e9 / 60.0));
            }
        }
    }

    SDL_ShowCursor(TRUE);
}

void OperatingSystem::enableDoubleBuffering()
{
    swapBuffers = true;
}

void OperatingSystem::enableSingleBuffering()
{
    swapBuffers = false;
}

Window& OperatingSystem::acquireWindow()
{
    return windowManager;
}

AssetManager * OperatingSystem::acquireAssetManager()
{
    return &assetManager;
}

SceneManager * OperatingSystem::acquireSceneManager()
{
    return &sceneManager;
}

int bootstrap(std::function<void(OperatingSystem *)> onApplicationStart)
{
    SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

    auto message { EXIT_SUCCESS };

    try 
    {
        OperatingSystem system;
        if (system.setupLibraries())
        {
            onApplicationStart(&system);
            system.releaseLibraries();
        }
        else
        {
            message = EXIT_FAILURE;
        }
    }
    catch (Error e) 
    {
        SDL_ShowSimpleMessageBox(0, e.title(), e.what(), nullptr);
        message = EXIT_FAILURE;
    }

    return message;
}