
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

#include "Silence.h"

// Constructor & Deconstructor
Menu::Menu(OperatingSystem * system)
{
	// Aquire the scene manager
	scenes = system->aquireSceneManager();
	// aquire a local asset manager for the scene

	AssetManager * assets = system->aquireAssetManager();
	package = assets->grabLocalManager();
	// and grab these assets from disk
	package->grab({ "data/textures/menu1.png",
		"data/textures/menu2.png",
		"data/textures/menu3.png",
		"data/fonts/Calibri.ttf",
		"data/textures/button.png"
	});

	menuMusic.open(package->grabMusic("data/media/menu.wav", STREAM_LOOP));
	roar.open(package->grabMusic("data/media/roar.mp3", LOAD));

	entered = false;
	exited = false;
}

Menu::~Menu()
{
	// Packages need to be manually deleted
	SAFE_RELEASE(package);
}

//
void Menu::onEnter(int ID)
{
	background.enter();

	if (ID == (int)SceneID::Gameover || ID == -1 || ID == (int)SceneID::Intro) {
		menuMusic.play();
	}

	entered = true;
	exited = false;
	alpha = 0.0f;
}

// Handles when the scene is created
void Menu::onCreate()
{
	background.create(package);
	headers.create(package);
	buttons.create(package);

	// finally create the 2D renderer
	renderer2D.createRenderer();
}

void Menu::onGamepadButton(int key, int state)
{
	buttons.onGamepadButton(key, state);
	for (int i = 0; i < 3; i++) {
		if (buttons.isPressed(i, key, state)) {
			switch (i) {
				case 0: menuMusic.stop(); roar.reset(); roar.play(); exited = true; return;
				case 1: scenes->switchScene((unsigned)SceneID::Options); return;
				case 2: scenes->exit(); return;

			default: break;
			}
		}
	}

	if (key == SDL_CONTROLLER_BUTTON_BACK && state == GAMEPAD_BUTTON_PRESSED) {
		scenes->exit();
	}
}

// Handle SDL / Game events
void Menu::onGameEvent(SDL_Event& e)
{
	background.event(e);
	headers.event(e);
	buttons.event(e);

	for (int i = 0; i < 3; i++) {
		if (buttons.isPressed(i, e)) {
			switch (i) {
				case 0:  menuMusic.stop(); roar.reset(); roar.play(); exited = true; return;
				case 1: scenes->switchScene(as_int(SceneID::Options)); return;
				case 2: scenes->exit(); return;

				default: break;
			}
		}
	}
}

// nothing needs updating so this can be empty
void Menu::onUpdate()
{
	background.update();
	headers.update();
	buttons.update();

	if (entered) {
		alpha += 0.05f;
		if (alpha >= 1.0)
			entered = false;
	}

	if (exited) {
		if (alpha < -0.5) {
			exited = false;
			scenes->switchScene(as_int(SceneID::Indoors));
		}

		alpha -= 0.005f;
	}
}

// Handles when the scene is rendered
void Menu::onRender()
{
	// create our matrices
	glm::mat4 projectionMatrix = glm::mat4();
	glm::mat4 modelMatrix = glm::mat4();

	// set the projection matrix
	projectionMatrix = glm::ortho(0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0);

	// then prepare our 2d render send out matrices over
	renderer2D.clear();
	renderer2D.prepare();
	renderer2D.setMatrixForObject("projection", projectionMatrix);
	renderer2D.setMatrixForObject("model", modelMatrix);

	if (entered == false && exited == false) {
		renderer2D.setAlpha(background.getAlpha());
	} else {
		renderer2D.setAlpha(alpha);
	}

	background.render(&renderer2D);
	renderer2D.setAlpha(alpha);

	headers.render(&renderer2D);
	buttons.render(&renderer2D);

	renderer2D.present();
}