
#pragma once

#include "InteractionLayerExport.h"
#include "EngineLayer.h"
#include "Gamepad.h"
#include "Window.h"

class Scene 
{
public:
    virtual void onGameEvent(SDL_Event& e){}
    virtual void onGamepadAxis(int, float){}
    virtual void onGamepadButton(int, int){}
    virtual void onRender(){}
    virtual void onCreate(){}
    virtual void onUpdate(){}
    virtual void onExit(int){}
    virtual void onEnter(int){}
    virtual ~Scene(){}
};

class SILENCE_EXPORT_API SceneManager
{
    std::vector<Scene*> engineScenes;
    Gamepad gamepad;
    Scene * globalScene;
    Window * window;
    int current;
    int start;
public:
    explicit SceneManager(Window * w);
    ~SceneManager();

    Gamepad * getGamepad();

    void packSceneList(std::initializer_list<Scene*> list);
    void setGlobalScene(Scene*);
    void handeEventScene(SDL_Event& e);
    void createScenes();
    void renderScene();
    void updateScene();
    void switchScene(unsigned int ID);
    void setStart(unsigned int ID);
    void exit();

    Scene * getScene(int ID);
};