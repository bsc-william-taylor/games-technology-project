
#include "Silence.h"
#include "intro.h"

Intro::Intro(OperatingSystem * os)
    : system(os)
{
}

Intro::~Intro()
{
    introVideo.unload();
}

void Intro::onUpdate()
{
    if (introTimer.elapsed(Milliseconds) >= 9000)
    {
        introTimer.clear();
        auto sceneManager = system->acquireSceneManager();
        system->enableDoubleBuffering();
        sceneManager->switchScene(int(SceneID::Menu));
    }
}

void Intro::onEnter(int)
{    
    system->enableSingleBuffering();
    system->acquireWindow().forceFullscreenMode();

    introTimer.start();
    introVideo.play();
}

void Intro::onCreate()
{
    introVideo.streamFrom("data/media/intro.wmv");
}

void Intro::onRender()
{
    introVideo.display();
}

void Intro::onGamepadButton(int k, int s)
{
}

void Intro::onGameEvent(SDL_Event& e)
{
}