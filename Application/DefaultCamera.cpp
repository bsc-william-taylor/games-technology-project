
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

#include "DefaultCamera.h"

// Constructor & Deconstructor
DefaultCamera::DefaultCamera()
	: viewMatrix(glm::mat4())
{
}

DefaultCamera::~DefaultCamera()
{
}

// Just returns a indentity matrix
glm::mat4& DefaultCamera::getView()
{
	return viewMatrix;
}

// Empty function as Default Camera == NO camera
void DefaultCamera::handleTerrainHeight(float f)
{
}

void DefaultCamera::cancelMovement()
{

}

// returns the center of the scene
glm::vec3 DefaultCamera::getPosition()
{
	return glm::vec3(0.0, 0.0, 0.0);
}

// Empty function as Default Camera == NO camera
void DefaultCamera::updateCameraPosition(SDL_Event&)
{
}

// Empty function as Default Camera == NO camera
void DefaultCamera::setPosition(glm::vec3)
{
}

// Empty function as Default Camera == NO camera
void DefaultCamera::repositionCamera()
{
}

// just return the default value of 0
GLfloat DefaultCamera::getHeight()
{
	return 0.0f;
}
