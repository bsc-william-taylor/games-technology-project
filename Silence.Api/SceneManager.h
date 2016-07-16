
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

#pragma once

#include "InteractionLayerExport.h"
#include "EngineLayer.h"
#include "Gamepad.h"
#include "Window.h"

class Scene {
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
private:
	std::vector<Scene*> engineScenes;
	unsigned int current;
	Gamepad gamepad;
	Window * window;
	Scene * globalScene;
	unsigned int start;
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