
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

#ifndef _MAIN_H_
#define _MAIN_H_

// an enum of IDs for each scene
enum class SceneID {
	Intro, Menu, Indoors, Forest, Gameover, Options
};

constexpr int as_int(SceneID id)
{
	return static_cast<int>(id);
}

#include "InteractionLayer.h"
#include "gameover.h"
#include "options.h"
#include "forest.h"
#include "indoors.h"
#include "intro.h"
#include "menu.h"

#pragma comment(lib, "Silence.Core.lib")
#pragma comment(lib, "Silence.Api.lib")

#endif