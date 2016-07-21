
/**
*
* Copyright (c) 2014 : William Taylor : wi11berto@yahoo.co.uk
*
* This software is provided 'as-is', without any
* express or implied warranty. In no event will
* the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/


#include "HighPrecisionTimer.h"
#include "System.h"

OperatingSystem::OperatingSystem()
    : sceneManager(&windowManager), swapBuffers(true)
{
}

OperatingSystem::~OperatingSystem()
{
}

Gamepad * OperatingSystem::aquireGamepad()
{
    return sceneManager.getGamepad();
}

bool OperatingSystem::setupLibraries()
{
    auto SDL_Params{ SDL_INIT_EVERYTHING };
    auto setupSuccess{ true };

    if (SDL_Init(SDL_Params) != NULL) 
    {
        throw Error(GAME, "Couldnt init SDL");
    }

    if (TTF_Init() != NULL) 
    {
        throw Error(GAME, "Couldnt init TTF");
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

            if (swapBuffers) windowManager.swapBuffers();

            if (!windowManager.vsyncEnabled())
            {
                while (frameTimer.elapsed(NS) <= static_cast<float>(1.0e9 / 60.0));
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

Window& OperatingSystem::aquireWindow()
{
    return windowManager;
}

AssetManager * OperatingSystem::aquireAssetManager()
{
    return &assetManager;
}

SceneManager * OperatingSystem::aquireSceneManager()
{
    return &sceneManager;
}

#include <ShellScalingApi.h>

int bootstrap(std::function<void(OperatingSystem *)> onApplicationStart)
{
    SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

    auto message{ EXIT_SUCCESS };

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
        message = EXIT_FAILURE;
        SDL_ShowSimpleMessageBox(0,
            e.title(),
            e.what(),
            nullptr
        );
    }

    return message;
}