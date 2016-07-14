
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

#include "FirstPersonCamera.h"

// Constructor & De constructor
FirstPersonCamera::FirstPersonCamera()
	: rotation(0.0), pitch(0.0), height(0.0F), speed(0.75)
{
	// set the initial values for all variables
	viewMatrix = glm::mat4(1.0);
	cameraArea = glm::vec4(-225, -135, 225, 135);

	translate = glm::vec3(0.0, -1000.0f, 0.0);
	last = glm::vec3(0.0, -1000.0f, 0.0);

	cancel = false;
	gamepadEnabled = false;
	enabled = true;
	// set default key state
	for (int i = 0; i < 4; i++) {
		keys[i] = FALSE;
		if (i < 2) axis[i] = FALSE;
	}

	terrainHeight = 0;
	velocity = 0.0f;
}

// Alternative copy constructor which copies all data from another camera
FirstPersonCamera::FirstPersonCamera(Camera * camera)
	: rotation(0.0), pitch(-50), height(0.0F), speed(0.75)
{
	cameraArea = glm::vec4(-225, -135, 225, 135);
	viewMatrix = camera->getView();
	translate = camera->getPosition();
	cancel = false;
	enabled = true;
	gamepadEnabled = false;
	// set default key state
	for (int i = 0; i < 4; i++) {
		keys[i] = FALSE;
		if (i < 2) axis[i] = FALSE;
	}

	translate = glm::vec3(0.0, -1000.0f, 0.0);
	last = glm::vec3(0.0, -1000.0f, 0.0);
	terrainHeight = 0;
	velocity = 0.0f;
}

GLvoid FirstPersonCamera::disable()
{
	enabled = false;
}

GLvoid FirstPersonCamera::enable()
{
	enabled = true;
}

glm::vec3 FirstPersonCamera::getCameraDirection()
{
	double z = glm::cos(glm::radians(rotation));
	double y = glm::cos(glm::radians(pitch+90));
	double x = glm::sin(glm::radians(rotation));	

	return glm::normalize(glm::vec3(x, y, -z));
}

FirstPersonCamera::~FirstPersonCamera()
{
}

// Returns the height of the camera
float FirstPersonCamera::getHeight()
{
	return height;
}

// returns the view matrix for the camera
glm::mat4& FirstPersonCamera::getView()
{
	return viewMatrix;
}

void FirstPersonCamera::onGamepadAxis(int axisID, float angle)
{
	if (axisID == SDL_CONTROLLER_AXIS_LEFTX) {
		axis[0] = GL_TRUE;
		axisV[0] += angle * speed;
	}

	if (axisID == SDL_CONTROLLER_AXIS_LEFTY) {
		axis[1] = GL_TRUE;
		axisV[1] += angle * speed;
	}

	if (axisID == SDL_CONTROLLER_AXIS_RIGHTX) {
		rotation += angle * 1.75;
	}

	if (axisID == SDL_CONTROLLER_AXIS_RIGHTY) {
		float next = pitch + (angle * 1.75);
		if (next <= 55.0 && next >= -55.0) {
			pitch += angle * 1.75;
		}
	}
}

void FirstPersonCamera::walk()
{
	speed = (0.75);
}

void FirstPersonCamera::run()
{
	speed = (1.1);
}

void FirstPersonCamera::cancelMovement()
{
	cancel = true;
}

// handles when the camera is at a point on the terrain
void FirstPersonCamera::handleTerrainHeight(float h)
{
	terrainHeight = h;
}

// returns the position of the camera
glm::vec3 FirstPersonCamera::getPosition()
{
	return -last;
}

// The following function handles updating the camera's position based on key pressed
void FirstPersonCamera::updateCameraPosition(SDL_Event& e)
{
	// if we have a keydown event
	if (e.type == SDL_KEYDOWN) 
	{
		// change the key state variable accordingly
		switch (e.key.keysym.sym) {
			case SDLK_w: keys[0] = TRUE;  break;
			case SDLK_s: keys[1] = TRUE;  break;
			case SDLK_a: keys[2] = TRUE;  break;
			case SDLK_d: keys[3] = TRUE;  break;
				default:
					break;
		}
	}

	// if we have a key up event
	if (e.type == SDL_KEYUP) 
	{
		// change the key state variable accordingly
		switch (e.key.keysym.sym) {
			case SDLK_w: keys[0] = FALSE;  break;
			case SDLK_s: keys[1] = FALSE;  break;
			case SDLK_a: keys[2] = FALSE;  break;
			case SDLK_d: keys[3] = FALSE;  break;
		default:
			break;
		}
	}

	// if we have a mouse motion event we rotate the camera and calculate the pitch of the camera as well
	if (e.type == SDL_MOUSEMOTION) 
	{
		// x magnitude = rotation
		rotation += e.motion.xrel;
		// y magnitude = rotation
		float next = pitch + e.motion.yrel;
		if (next <= 55.0 && next >= -55.0) {
			pitch += e.motion.yrel;
		}
		
		// make sure to keep the degrees in readable form
		if (rotation < 0) {
			rotation = 360;
		}

		if (rotation > 360) {
			rotation = 0;
		}
	}
}

// the following sets the new position of the camera
void FirstPersonCamera::setPosition(glm::vec3 position)
{
	for (int i = 0; i < 4; i++) {
		keys[i] = FALSE;
		if (i < 2) axis[i] = FALSE;
	}

	// calculate where to translate the camera to
	translate = -position;
	translate.z = position.z;
	
	// calculate the height of the camera
	height = -position.y;
	terrainHeight = 0;
	cancel = false;
}

void FirstPersonCamera::prepareCamera()
{
	static float f = 0.0f;
	static bool s = true;

	if (velocity > 0.1) {
		if (speed >= 1.0) {
			if (f >= 0.5f) {
				s = false;
			}

			if (f <= -0.5f) {
				s = true;
			}

			if (s) {
				f += 0.05;
			}
			else {
				f -= 0.05;
			}
		}
		else {
			f = 0.0;
		}
	} else {
		f = 0.0;
	}
	
	// reset the view matrix
	viewMatrix = glm::mat4(1.0);
	viewMatrix = glm::rotate(viewMatrix, pitch, glm::vec3(1.0, 0.0, 0.0));
	viewMatrix = glm::rotate(viewMatrix, rotation, glm::vec3(0.0, 1.0, 0.0));
	viewMatrix = glm::rotate(viewMatrix, f, glm::vec3(0.0, 0.0, 1.0));

	// then calculate the temporary position
	last = glm::vec3(translate);

	last.y += ((terrainHeight + height) - last.y) / 10.0;

	// calculate the direction of travel
	if (keys[0]) {
		last += glm::vec3(glm::sin(glm::radians(-rotation)), 0.0, glm::cos(glm::radians(-rotation))) * speed;
	} 
	
	if (keys[1]) {
		last += glm::vec3(-glm::sin(glm::radians(-rotation)), 0.0, -glm::cos(glm::radians(-rotation)))  * speed;
	} 
	
	if (keys[2]) {
		last += glm::vec3(glm::cos(glm::radians(-rotation)), 0.0, -glm::sin(glm::radians(-rotation)))  * speed;
	}

	if (keys[3]) {
		last += glm::vec3(-glm::cos(glm::radians(-rotation)), 0.0, glm::sin(glm::radians(-rotation)))  * speed;
	}

	if (axis[0]) {
		last += glm::vec3(glm::cos(glm::radians(-rotation)), 0.0, -glm::sin(glm::radians(-rotation)))  * -axisV[0];
		axisV[0] = 0.0;
		axis[0] = FALSE;
	}

	if (axis[1]) {
		last += glm::vec3(glm::sin(glm::radians(-rotation)), 0.0, glm::cos(glm::radians(-rotation))) * -axisV[1];
		axisV[1] = 0.0;
		axis[1] = FALSE;
	}
}

void FirstPersonCamera::setCameraArea(glm::vec4 vec)
{
	cameraArea = vec;
}

// this function applies the new position to the view matrix
void FirstPersonCamera::repositionCamera()
{
	velocity = glm::length(translate - last);
	
	// and only apply the new position if within a given area
	if (last.x >= cameraArea.x && last.x <= cameraArea.z && 
		last.z >= cameraArea.y && last.z <= cameraArea.w && 
		!cancel && enabled)
	{
		translate = last;
	}

	// apply the transformation
	viewMatrix = glm::translate(viewMatrix, translate);
	cancel = false;

	if (gamepadEnabled) {
		for (int i = 0; i < 4; i++) {
			keys[i] = FALSE;
		}
	}
}