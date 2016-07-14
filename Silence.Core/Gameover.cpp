
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

#include "applicationLayer.h"
#include <iomanip>

std::string toStrMaxDecimals(double value, int decimals)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(decimals) << value;
	std::string s = ss.str();
	if (decimals > 0 && s[s.find_last_not_of('0')] == '.') {
		s.erase(s.size() - decimals + 1);
	}

	return s;
}

// Constructor & Deconstructor
Gameover::Gameover(OperatingSystem * system)
{
	// Aquire the scene manager
	// aquire a local asset manager for the scene
	package = system->aquireAssetManager()->grabLocalManager();
	// and grab these assets from disk
	package->grab({
		"data/textures/button.png",
		"data/textures/menu3.png",
		"data/textures/box.png"
	});

	scenes = system->aquireSceneManager();
}

Gameover::~Gameover()
{
	// Packages need to be manually deleted
	SAFE_RELEASE(package);
}

void Gameover::onEnter(int i)
{
	survivorTimer = ((Forest *)scenes->getScene((int)SceneID::Forest))->getForestTime();

	timeSurvived = "Time Survived : " + toStrMaxDecimals(survivorTimer->elapsed(SECONDS), 2).append("s");
}

// Handles when the scene is created
void Gameover::onCreate()
{
	// Load all assets starting with the header
	background.setTexture(package->getT("data/textures/menu3"));
	background.setArea(glm::vec4(0, 0, 1920, 1080));
	
	boxart.setTexture(package->getT("data/textures/box"));
	boxart.setArea(glm::vec4(1100, 220, 700, 470));

	header.setFont(package->getL("data/fonts/Calibri", 200, { 255, 255, 255 }), "Thanks for Playing");
	header.setArea(glm::vec2(1920 / 2, 900), ALIGNMENT::CENTER);

	survivorTimer = ((Forest *)scenes->getScene((int)SceneID::Forest))->getForestTime();
	std::string timeSurvived = "Time Survived : " + toStrMaxDecimals(survivorTimer->elapsed(SECONDS), 2).append("s");

	time.setFont(package->getL("data/fonts/Calibri", 50, { 255, 255, 255 }), timeSurvived.c_str());
	time.setArea(glm::vec2(200, 550), ALIGNMENT::RIGHT);

	message.setFont(package->getL("data/fonts/Calibri", 50, { 255, 255, 255 }), "Look for the full game sometime soon");
	message.setArea(glm::vec2(200, 450), ALIGNMENT::RIGHT);

	back.setButtonTexture(package->getT("data/textures/button"));
	back.setButtonText(package->getL("data/fonts/Calibri", 40, { 255, 255, 255 }), "Back to Menu");
	back.setArea(glm::vec4(400, 200, 400, 225), ALIGNMENT::CENTER);

	renderer2D.createRenderer();
}

void Gameover::onGamepadButton(int key, int state)
{
	if (key == SDL_CONTROLLER_BUTTON_A && state == GAMEPAD_BUTTON_PRESSED)
	{
		scenes->switchScene((unsigned)SceneID::Menu);
	}

	if (key == SDL_CONTROLLER_BUTTON_B && state == GAMEPAD_BUTTON_PRESSED)
	{
		scenes->switchScene((unsigned)SceneID::Menu);
	}
}

// Handle SDL / Game events
void Gameover::onGameEvent(SDL_Event& e)
{
	if (back.isPressed(e))
	{
		scenes->switchScene((unsigned)SceneID::Menu);
	}
}

// nothing needs updating so this can be empty
void Gameover::onUpdate()
{
	time.setFont(package->getL("data/fonts/Calibri", 50, { 255, 255, 255 }), timeSurvived.c_str());
	time.setArea(glm::vec2(200, 550), ALIGNMENT::RIGHT);
}

// Handles when the scene is rendered
void Gameover::onRender()
{
	glm::mat4 projectionMatrix = glm::mat4();
	glm::mat4 modelMatrix = glm::mat4();

	projectionMatrix = glm::ortho(0.0, 1920.0, 0.0, 1080.0);

	renderer2D.prepare();
	renderer2D.setAlpha(1.0f);
	renderer2D.setMatrixForObject("projection", projectionMatrix);
	renderer2D.setMatrixForObject("model", modelMatrix);
	renderer2D.renderTexture(&background);
	renderer2D.renderTexture(&boxart);
	renderer2D.renderLabel(&message);
	renderer2D.renderLabel(&header);
	renderer2D.renderLabel(&time);
	renderer2D.renderButton(&back);
	renderer2D.present();
}