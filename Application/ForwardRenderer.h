
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

// long list of includes however the class uses many objects
#include "PerFragmentShader.h"
#include "FirstPersonCamera.h"
#include "GeometryBuffer.h"
#include "DefaultCamera.h"
#include "Heightmap.h"
#include "ModelSet.h"
#include "md2Model.h"
#include "Skybox.h"
#include "Lights.h"
#include "Model.h"
#include "Cube.h"
#include "GPU.h"

// This class is a renderer which renders the object using the deferred rendering technique 
class ForwardRenderer
{
private:
	// our output shader which is the shader used during the second pass
	PerFragmentShader shader;
	// our camera for the scene
	Camera * sceneCamera;
	//
	bool fogEnabled;
	// and a state variable to tell the camera type
	CAMERA cameraType;
	//
	Lights lights;
public:
	// Constructor & Deconstructor
	ForwardRenderer();
	~ForwardRenderer();

	Lights * getLights() {
		return &lights;
	}

	void setCameraDirection(float, float);
	
	void enableFog();
	void disableFog();

	//
	void disableWireframeMode();
	//
	void enableWireframeMode();
	//
	void setProjectionMatrix(GPU_Matrices& matrices);
	// This function sets a matrix variable inside the program
	void setModelMatrix(GPU_Matrices& matrices);
	// the function will get the terrain height and apply it to the camera
	void handleTerrainHeight(Heightmap * heightmap);
	// this function will set the camera position inside the scene
	void setCameraPosition(glm::vec3 positions);
	// this function will change the camera type
	void changeCamera(CAMERA newCamera);
	// this function will create the renderer for a specific screen width and height
	void createRenderer();
	// this function will handle a SDL event and apply the correct response for the camera
	void updateCamera(SDL_Event& event);
	// this function updates the view matrix for the scene
	void repositionCamera();
	//
	void setAlpha(float);
	//
	void onGamepadAxis(int i, float x);
public:
	// this function renders a skybox in 3D space
	void renderSkybox(Skybox * skybox);
	// renders animated model
	void renderAnimatedModel(AnimatedModel * model);
	//
	void renderModelSet(ModelSet * set);
	// this function renders a heightmap in 3D space
	void renderMap(Heightmap * map);
	// this function renders a model in 3D space
	void renderModel(Model * model);
	// this function renders a cube in 3D space
	void renderCube(Cube *  cube);
	//
	void setCameraArea(glm::vec4);
	// this function prepares the renderer for rendering objects
	void prepare();
	// this function presents the final image
	void present();

	// returns the type of camera being used in the scene
	CAMERA getCameraType();

	Camera * getCamera() {
		return sceneCamera;
	}
};