
#include "Silence.h"

int main(int argc, char * argv[])
{
    return bootstrap([&](OperatingSystem * operatingSystem) 
    {
        auto window = &operatingSystem->aquireWindow();

        window->setPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        window->setWindowIcon("data/textures/icon.bmp");
        window->setTitle("Silence : Enter The Forest");
        window->setSize(1280, 720);
        window->create();

        auto scenes = operatingSystem->aquireSceneManager();

        scenes->setStart(int(SceneID::Intro));
        scenes->packSceneList(
        {
            new Intro(operatingSystem),
            new Menu(operatingSystem),
            new Indoors(operatingSystem),
            new Forest(operatingSystem),
            new Gameover(operatingSystem),
            new Options(operatingSystem)
        });

        operatingSystem->executeProgram();
    });
}