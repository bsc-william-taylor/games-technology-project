
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
#include "DeferredOutputShader.h"
#include "DeferredInputShader.h"
#include "FirstPersonCamera.h"
#include "GeometryBuffer.h"
#include "DefaultCamera.h"
#include "Heightmap.h"
#include "md2Model.h"
#include "Skybox.h"
#include "Model.h"
#include "Cube.h"
#include "GPU.h"

// This class is a renderer which renders the object using the deferred rendering technique 
class SILENCE_EXPORT DeferredRenderer
{
private:
	// our output shader which is the shader used during the second pass
	DeferredOutputShader secondPassShader;
	// out input shader which is used in the first pass
	DeferredInputShader firstPassShader;
	// our geometry buffer for rendering geometry into
	GeometryBuffer * gbuffer;
	// our camera for the scene
	Camera * sceneCamera;
	// and a state variable to tell the camera type
	CAMERA cameraType;
public:
	// Constructor & Deconstructor
	DeferredRenderer();
	~DeferredRenderer();

	void setRenderRegion(int, int);
	// This function sets a matrix variable inside the program
	void setMatrixForObject(const char *, glm::mat4&);
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
	void onGamepadAxis(int i, float x);
public:
	// this function accepts a list of lights and pushes them into the scene
	//void pushLights(std::vector<PointLight>& pointLights);
	// this function renders a skybox in 3D space
	void renderSkybox(Skybox * skybox);
	// renders animated model
	void renderAnimatedModel(AnimatedModel * model);
	// this function renders a heightmap in 3D space
	void renderMap(Heightmap * map);
	// this function renders a model in 3D space
	void renderModel(Model * model);
	// this function renders a cube in 3D space
	void renderCube(Cube *  cube);
	//
	void setCameraArea(glm::vec4);
	// this function finalizes light data and sends it to the UBO
	void constructUBO();
	// this function prepares the renderer for rendering objects
	void prepare();
	// this function presents the final image
	void present();

	// returns the type of camera being used in the scene
	CAMERA getCameraType();

	Camera * getCamera() {
		return sceneCamera;
	}

	// returns a reference to the directional lights position
	glm::vec3& getLightPosition();
	// returns a reference to the directional lights colour
	glm::vec3& getLightColour();
private:
	//
	void renderVolume(int, int, glm::mat4, glm::mat4);
	// The transfer object for the sphere (used for light volumes)
	Model * sphere;
	// The transfer object for the quad (used for the final image)
	GPU_Transfer * quad;
	// our UBO light ID
	GPU_ID lID;
private:
	// This is our vector of point lights for the scene
	//std::vector<PointLight> pointLights;
	// this is the sphere volume used for light columns
	ModelAsset sphereAsset;
	
	// final these 2 variables are for the directional light
	glm::vec3 lightPosition;
	glm::vec3 lightColour;
};