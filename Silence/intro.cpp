
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
	if (introTimer.elapsed(MS) >= 9000)
	{
		introTimer.clear();
		auto sceneManager = system->aquireSceneManager();
		system->enableDoubleBuffering();
		sceneManager->switchScene(as_int(SceneID::Menu));
	}
}

void Intro::onEnter(int previous)
{
	system->enableSingleBuffering();
	system->aquireWindow().forceFullscreenMode();

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