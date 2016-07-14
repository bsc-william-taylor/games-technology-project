
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

	if (globalScene != NULL)
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
	glClearColor(0.0, 0.0, 0.0, 0.0);

	gamepad.open();

	if (gamepad.isConnected())
	{
		SDL_ShowCursor(FALSE);
	}

	if (!engineScenes.empty())
	{
		for(auto& e : engineScenes) {
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
		int previous = current;
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