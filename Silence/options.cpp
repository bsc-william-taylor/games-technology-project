
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
Options::Options(OperatingSystem * system)
{
    // Aquire the scene manager
    scenes = system->acquireSceneManager();
    // aquire a local asset manager for the scene
    package = system->acquireAssetManager()->grabLocalManager();
    // and grab these assets from disk
    package->grab({ "data/textures/options.png",
        "data/fonts/Calibri.ttf",
        "data/textures/button.png",
        "data/textures/back.png",
        "data/textures/right.png",
        "data/textures/left.png"
    });

    optionInputs = new OptionsField(&system->acquireWindow());
}

Options::~Options()
{
    // Packages need to be manually deleted
    SAFE_RELEASE(package);
}

// Handles when the scene is created
void Options::onCreate()
{
    optionInputs->create(package);

    // Load all assets starting with the header
    header.setFont(package->getL("data/fonts/Calibri", 225, { 255, 255, 255 }), "Options");
    header.setArea(glm::vec2(1920 / 2, 900), ALIGNMENT::CENTER);

    version.setFont(package->getL("data/fonts/Calibri", 25, { 255, 255, 255 }), "Version 1.0");
    version.setArea(glm::vec2(1920 / 2, 100), ALIGNMENT::CENTER);

    // Load the play button set the font, text and location
    backButton.setButtonTexture(package->getT("data/textures/back"));
    backButton.setArea(glm::vec4(10, 945, 125, 125), ALIGNMENT::RIGHT);

    // Load the background texture and make it fullscreen
    background.setTexture(package->getT("data/textures/options"));
    background.setArea(glm::vec4(0.0, 0.0, 1920, 1080));

    // finally create the 2D renderer
    renderer2D.createRenderer();
}

void Options::onGamepadButton(int key, int state)
{
    if ((key == SDL_CONTROLLER_BUTTON_B || key == SDL_CONTROLLER_BUTTON_BACK) && state == GamepadButtonPressed) {
        scenes->switchScene((unsigned)SceneID::Menu);
    }

    optionInputs->onGamepadButton(key, state);	
}

// Handle SDL / Game events
void Options::onGameEvent(SDL_Event& e)
{
    optionInputs->onGameEvent(e);

    // if the play button has been pressed
    if (backButton.isPressed(e))
    {
        // go to the lvl scene
        scenes->switchScene((unsigned)SceneID::Menu);
    }
}

// nothing needs updating so this can be empty
void Options::onUpdate()
{
    optionInputs->update();
}

// Handles when the scene is rendered
void Options::onRender()
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
    renderer2D.setAlpha(1.0f);

    // then render each object in order
    renderer2D.renderTexture(&background);
    renderer2D.renderButton(&backButton);

    //
    optionInputs->render(&renderer2D);
    
    //
    renderer2D.renderLabel(&version);
    renderer2D.renderLabel(&header);
    renderer2D.present();
}