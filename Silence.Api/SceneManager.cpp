
#include "SceneManager.h"

SceneManager::SceneManager(Window * w) 
{
    engineScenes.reserve(100);
    globalScene = nullptr;
    current = NULL;
    start = NULL;
    window = w;
}

SceneManager::~SceneManager()
{	
}

Scene * SceneManager::getScene(int ID)
{
    return engineScenes[ID];
}

Gamepad * SceneManager::getGamepad()
{
    return &gamepad;
}

void SceneManager::setGlobalScene(Scene * scene)
{
    globalScene = scene;
}

void SceneManager::setStart(unsigned int ID)
{
    current = ID;
    start = ID;
}

void SceneManager::packSceneList(std::initializer_list<Scene *> scenes)
{
    for (auto& scene : scenes)
    {
        engineScenes.push_back(scene);
    }
}

void SceneManager::renderScene()
{
    if (!engineScenes.empty()) 
    {
        engineScenes[current]->onRender();
    }

    if (globalScene != nullptr)
    {
        globalScene->onRender();
    }
}

void SceneManager::handeEventScene(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        SDL_Event exit;
        exit.type = SDL_QUIT;
        SDL_PushEvent(&exit);
    }

    if (e.type == SDL_MOUSEMOTION || e.button.button == SDL_BUTTON_LEFT)
    {
        auto sz = window->getWindowDimensions();

        e.motion.x *= (float)1920 / sz.w;
        e.motion.y *= (float)1080 / sz.h;
        e.motion.y = 1080 - e.motion.y;
    }

    if (e.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        auto sz = window->getWindowDimensions();
        glViewport(0, 0, sz.w, sz.h);
    }
    
    if (!engineScenes.empty())
    {
        engineScenes[current]->onGameEvent(e);
    }

    if (globalScene != nullptr)
    {
        globalScene->onGameEvent(e);
    }
}

void SceneManager::exit()
{
    SDL_Event exit;
    exit.type = SDL_QUIT;
    SDL_PushEvent(&exit);
}

void SceneManager::createScenes()
{
    gamepad.open();

    if (gamepad.isConnected())
    {
        SDL_ShowCursor(FALSE);
    }

    if (!engineScenes.empty())
    {
        for(auto& e : engineScenes) 
        {
            e->onCreate();
        }

        engineScenes[start]->onEnter(-1);
    }

    if (globalScene != nullptr)
    {
        globalScene->onCreate();
    }
}

void SceneManager::switchScene(unsigned int ID)
{
    if (ID < engineScenes.size() && ID > 0) 
    {
        auto previous = current;
        engineScenes[previous]->onExit(ID);
        current = ID;
        engineScenes[current]->onEnter(previous);
    }
}

void SceneManager::updateScene()
{
    if (gamepad.isConnected())
    {
        gamepad.getState();

        for (auto i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) 
        {
            engineScenes[current]->onGamepadButton(i, gamepad.getButtonValue(i));
        }

        for (auto i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++)
        {
            engineScenes[current]->onGamepadAxis(i, gamepad.getAxisValue(i));
        }
    }

    if (globalScene != nullptr) 
    {
        globalScene->onUpdate();
    }
    
    if (!engineScenes.empty()) 
    {
        engineScenes[current]->onUpdate();
    }
}