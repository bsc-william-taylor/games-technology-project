
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

#ifndef __FRAMEWORK_H_
#define __FRAMEWORK_H_

// Includes the following headers
#include <Windows.h>
#include <algorithm>
#include <functional>
#include <random>
#include <iostream>
#include <fstream>
#include <exception>
#include <sstream>
#include <dshow.h>
#include <d3d9.h>
#include <math.h>
#include <Evr.h>
#include <queue>
#include <string>
#include <map>
#include <memory>
#include <time.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <map>

// these are additional libraries that are not part of the C++ language
#include "../Silence.Libs/SDL2/SDL.h"
#include "../Silence.Libs/glew/glew.h"
#include "../Silence.Libs/assimp/postprocess.h"   
#include "../Silence.Libs/assimp/Importer.hpp"          
#include "../Silence.Libs/assimp/scene.h"   
#include "../Silence.Libs/freeimage/FreeImage.h"
#include "../Silence.Libs/SDL2/SDL_ttf.h"
#include "Exception.h"
#include "../Silence.Libs/bass/bass.h"

#include "Video.h"
#include <gl\gl.h>
// disable these warnings because there too F&*(^&* ANNOYING 
#pragma warning(disable : 4305)
#pragma warning(disable : 4018)
#pragma warning(disable : 4244)
#pragma warning(disable : 4251)
#pragma warning(disable : 4101)
#pragma warning(disable : 4996)
#pragma warning(disable : 4800)

// Link to the libraries as they have been included
#pragma comment(lib, "../Silence.Libs/Freeimage/FreeImage.lib")
#pragma comment(lib, "../Silence.Libs/SDL2/SDL2main.lib")
#pragma comment(lib, "../Silence.Libs/SDL2/SDL2_ttf.lib")
#pragma comment(lib, "../Silence.Libs/assimp/assimp.lib") 
#pragma comment(lib, "../Silence.Libs/SDL2/SDL2.lib")
#pragma comment(lib, "../Silence.Libs/glew/glew32.lib")
#pragma comment(lib, "../Silence.Libs/bass/bass.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Shcore.lib")

// include GLM until I have the time to write my own math classes
#include "../Silence.Libs/glm/glm.hpp"
#include "../Silence.Libs/glm/gtc/matrix_transform.hpp"
#include "../Silence.Libs/glm/gtc/type_ptr.hpp"

// some simple macros to help with certain things
#ifndef NULL
    #define NULL 0
#endif

#ifndef SAFE_RELEASE
    #define SAFE_RELEASE(ptr) if(ptr) { delete ptr; ptr = 0; }
#else
    #error("SAFE_RELEASE macro already supplied")
#endif

#ifndef PI
    #define PI 3.14159265359F
#else
    #error("PI macro already supplied")
#endif

#pragma warning(disable: 4290)
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)

#if defined DLL_EXPORT 
    #define SILENCE_EXPORT __declspec(dllexport)
#else
    #define SILENCE_EXPORT __declspec(dllimport)
#endif

#endif