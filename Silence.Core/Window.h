
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

#ifndef __WINDOW__H_
#define __WINDOW__H_

#include "EngineLayer.h"

// This class sets up and manages a window for a GL application
class SILENCE_EXPORT Window
{
private:
	// The openGL context for the window
	SDL_GLContext windowContext;
	// The window itself
	SDL_Window * window;
	// The size of the window as well as its position
	SDL_Rect dimensions;
	// the title of the window
	const char * title;
	//
	std::string icon;
	// and should the window be fullscreen
	bool fullscreen;
	bool vsync;
public:
	// Constructor & Deconstructor
	Window();
	~Window();

	bool isFullscreen();

	//
	SDL_Rect getWindowDimensions();
	//
	void resizeWindow(int x, int y, int w, int h);
	// This function sets the position of the window
	void setPosition(int x, int y);
	// This function sets the title of the window
	void setTitle(const char * t);
	// This function sets the size of the window
	void setSize(int w, int h);
	// This function will make the window a fullscreen window when its constructed
	void makeFullscreen();
	// This function swaps the back buffer with the front buffer bring a rendered image to the front
	void swapBuffers();
	// This function creates the window make sure to set the properties first
	void create();
	//
	void forceFullscreenMode(int, int);
	//
	void forceFullscreenMode();
	//
	void adjustViewport();
	// 
	void setWindowIcon(std::string);
	//
	void forceWindowedMode();

	// returns the height and the width of the window
	int getMaxHeight();
	int getMaxWidth();
	int getHeight();
	int getWidth();

	// returns a direct point to the window object SDL provides
	SDL_Window * getWindow();

	//
	bool vsyncEnabled();
private:
	int maxHeight;
	int maxWidth;
};

#endif