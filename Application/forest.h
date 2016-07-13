
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


#ifndef __FOREST__H_
#define __FOREST__H_

#include "ForwardRenderer.h"
#include "System.h"
#include "World.h"
#include "Media.h"
#include "Actors.h"
#include "HLGI.h"
#include "Pickups.h"

// Our main demo level scene class
class Forest : public Scene
{
private:
	HighPrecisionTimer survivalTimer;
	LocalAssetManager * package;
	ForwardRenderer renderer3D;
	SceneManager * manager;
	Pickups pickups;
	World world;

	Music torchSound;
	Music footSteps;
	Music roar;
	Music sign;

	Matrices matrices;
	Terrain terrain;
	Monster monster;
	House house;
	Gate gate;
	Key key;
public:
	// Constructor & Deconstructor
	Forest(OperatingSystem * os);
	~Forest();

	void onGamepadButton(int key, int st);
	void onGamepadAxis(int, float);
	void onGameEvent(SDL_Event& e);
	void onCreate();
	void onUpdate();
	void onRender();

	void onEnter(int i);
	void onExit(int i);

	HighPrecisionTimer * getForestTime() {
		return &survivalTimer;
	}
private:
	Music backgroundMusic;
	float alpha;
	
	DirectionalLight nightLight;
	PointLight point;
	SpotLight torch;
	Player * player;

	HighPrecisionTimer timer;
	Gamepad * gamepad;
};

#endif