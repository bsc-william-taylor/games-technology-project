
#include "Silence.h"

Menu::Menu(OperatingSystem * system)
    : alpha(0.0)
{
    scenes = system->acquireSceneManager();
    package = system->acquireAssetManager()->grabLocalManager();
    package->grab({  
        "data/textures/menu1.png",
        "data/textures/menu2.png",
        "data/textures/menu3.png",
        "data/fonts/Calibri.ttf",
        "data/textures/button.png"
    });

    menuMusic.open(package->newAudio("data/media/menu.wav", StreamLoop));
    roar.open(package->newAudio("data/media/roar.mp3", Load));
    entered = false;
    exited = false;
}

Menu::~Menu()
{
    SAFE_RELEASE(package);
}

void Menu::onEnter(int ID)
{
    background.enter();

    if (ID == static_cast<int>(SceneID::Gameover) || ID == -1 || ID == static_cast<int>(SceneID::Intro)) 
    {
        menuMusic.play();
    }

    entered = true;
    exited = false;
    alpha = 0.0f;
}

void Menu::onCreate()
{
    background.create(package);
    headers.create(package);
    buttons.create(package);

    renderer2D.createRenderer();
}

void Menu::onGamepadButton(int key, int state)
{
    buttons.onGamepadButton(key, state);

    for (auto i = 0; i < 3; i++) 
    {
        if (buttons.isPressed(i, key, state)) 
        {
            switch (i) 
            {
                case 0: menuMusic.stop(); roar.reset(); roar.play(); exited = true; return;
                case 1: scenes->switchScene(static_cast<int>(SceneID::Options)); return;
                case 2: scenes->exit(); return;

                default: 
                    break;
            }
        }
    }

    if (key == SDL_CONTROLLER_BUTTON_BACK && state == GamepadButtonPressed) 
    {
        scenes->exit();
    }
}

void Menu::onGameEvent(SDL_Event& e)
{
    background.event(e);
    headers.event(e);
    buttons.event(e);

    for (auto i = 0; i < 3; i++) 
    {
        if (buttons.isPressed(i, e)) 
        {
            switch (i) 
            {
                case 0: menuMusic.stop(); roar.reset(); roar.play(); exited = true; return;
                case 1: scenes->switchScene(int(SceneID::Options)); return;
                case 2: scenes->exit(); return;

                default: break;
            }
        }
    }
}

void Menu::onUpdate()
{
    background.update();
    headers.update();
    buttons.update();

    if (entered) 
    {
        alpha += 0.05f;
       
         if (alpha >= 1.0)
        {
            entered = false;
        }
    }

    if (exited) 
    {
        if (alpha < -0.5) 
        {
            exited = false;
            scenes->switchScene(int(SceneID::Indoors));
        }

        alpha -= 0.005f;
    }
}

void Menu::onRender()
{
    auto projectionMatrix = glm::mat4();
    auto modelMatrix = glm::mat4();

    projectionMatrix = glm::ortho(0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0);

    renderer2D.clear();
    renderer2D.prepare();
    renderer2D.setMatrixForObject("projection", projectionMatrix);
    renderer2D.setMatrixForObject("model", modelMatrix);

    if (entered == false && exited == false) 
    {
        renderer2D.setAlpha(background.getAlpha());
    } 
    else 
    {
        renderer2D.setAlpha(alpha);
    }

    background.render(&renderer2D);
    renderer2D.setAlpha(alpha);

    headers.render(&renderer2D);
    buttons.render(&renderer2D);

    renderer2D.present();
}