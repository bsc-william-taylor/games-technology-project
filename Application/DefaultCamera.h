
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

#include "Camera.h"

// This class implements a default camera but actually for now just leaves anything empty
// Default Camera == No Camera
class DefaultCamera : public Camera 
{
private:
	glm::mat4 viewMatrix;
public:
	// Constructor & Deconstructor
	DefaultCamera();
	~DefaultCamera();
	
	// returns the position of the camera
	glm::vec3 getPosition();
	// returns the view matrix for the camera
	glm::mat4& getView();

	// updates the camera based on the keys pressed
	void updateCameraPosition(SDL_Event&);
	// updates the height of the camera based on the height of the terrain
	void handleTerrainHeight(float f);
	// sets the position of the camera
	void setPosition(glm::vec3);
	// updates the view matrix to match the position stored in the vector provided 
	void repositionCamera();

	void cancelMovement();

	void prepareCamera(){}

	glm::vec3 getCameraDirection() {
		return glm::vec3(0.0, 0.0, 0.0);
	}

	//
	void onGamepadAxis(int, float){}

	// returns the height of the camera in 3D space
	GLfloat getHeight();
};