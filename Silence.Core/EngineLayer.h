
#ifndef __FRAMEWORK_H_
#define __FRAMEWORK_H_

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

#include "../Silence.Libs/glm/glm.hpp"
#include "../Silence.Libs/glm/gtc/matrix_transform.hpp"
#include "../Silence.Libs/glm/gtc/type_ptr.hpp"

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