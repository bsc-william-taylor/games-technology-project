
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

#ifndef __CAMERA__H_
#define __CAMERA__H_

#include "EngineLayer.h"
#include "math.h"

// The following is an interface which all other camera must follow
class SILENCE_EXPORT Camera {
public:
	// any scene camera must implement all of the following functions
	virtual void updateCameraPosition(SDL_Event& w) = 0;
	virtual void handleTerrainHeight(GLfloat h) = 0;
	virtual void setCameraArea(glm::vec4){}
	virtual void setPosition(glm::vec3 p) = 0;
	virtual glm::vec3 getPosition() = 0;
	virtual void prepareCamera() = 0;
	virtual void repositionCamera() = 0;
	virtual glm::mat4& getView() = 0;
	virtual void cancelMovement() = 0;
	virtual void onGamepadAxis(int, float) = 0;
	virtual float getHeight() = 0;
	virtual void setCameraDirection(float, float) {}
	virtual ~Camera() {}

	virtual glm::vec3 getCameraDirection() = 0;

	virtual float getRotation(){ return 0.0; }
};

// The type of cameras available to the application
enum CAMERA {
	FIRST_PERSON, DEFAULT_CAMERA
};

#endif