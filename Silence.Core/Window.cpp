
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

#include "Window.h"

// Constructore & Deconstructor
Window::Window()
    : fullscreen(false),
      window(NULL),
      title(""),
      icon("")
{
    vsync = false;
}

Window::~Window()
{
    // If the window was created
    if (window != NULL) 
    {
        // delete the windows context
        SDL_GL_DeleteContext(windowContext);
        // and cleanup any memory loaded by SDL for the window
        SDL_DestroyWindow(window);
    }
}

// returns the height of the window
int Window::getHeight()
{
    return dimensions.h;
}

void Window::setWindowIcon(std::string filename)
{
    icon = filename;
}

// returns the width of the window
int Window::getWidth()
{
    return dimensions.w;
}

// sets the window position when it is created (SDL Macros will still work here)
void Window::setPosition(int x, int y)
{
    dimensions.x = x;
    dimensions.y = y;
}

// The following sets the title for the window (Note only seen in windowed mode)
void Window::setTitle(const char * t)
{
    title = t;
}

// sets the window size when it is created (SDL Macros will still work here)
void Window::setSize(int w, int h)
{
    dimensions.w = w;
    dimensions.h = h;
}

void Window::forceFullscreenMode(int w, int h)
{
    if (!isFullscreen())
    {
        fullscreen = true;
        setSize(w, h);

        SDL_SetWindowPosition(window, 0, 0);
        SDL_SetWindowSize(window, w, h);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
}

bool Window::isFullscreen()
{
    return this->fullscreen;
}

void Window::forceFullscreenMode()
{
    if (!isFullscreen())
    {
        makeFullscreen();

        SDL_SetWindowSize(window, dimensions.w, dimensions.h);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
}

void Window::forceWindowedMode()
{
    if (isFullscreen())
    {
        fullscreen = false;
        setSize(1280, 720);

        SDL_SetWindowSize(window, 1280, 720);
        SDL_SetWindowFullscreen(window, NULL);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
}

void Window::adjustViewport()
{
    glViewport(0, 0, dimensions.w, dimensions.h);
}

// Swaps the buffers for the GL window
void Window::swapBuffers()
{
    SDL_GL_SwapWindow(window);
}

void Window::resizeWindow(int x, int y, int w, int h)
{
    SDL_SetWindowSize(window, w, h);
    SDL_SetWindowPosition(window, x, y);

    dimensions.x = x;
    dimensions.y = y;
    dimensions.w = w;
    dimensions.h = h;
}

SDL_Rect Window::getWindowDimensions()
{
    SDL_Rect size;

    int w = 0;
    int h = 0;
    int x = 0;
    int y = 0;

    // Get the windows size
    SDL_GetWindowSize(window, &w, &h);
    SDL_GetWindowPosition(window, &x, &y);

    size.x = x;	size.y = y;
    size.w = w;	size.h = h;

    return size;
}

int Window::getMaxHeight()
{
    return maxHeight;
}

int Window::getMaxWidth()
{
    return maxWidth;
}

// The following creates the window with the specified properties
void Window::create()
{
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    maxHeight = current.h;
    maxWidth = current.w;

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);

    // if the user wanted a normal window
    if (!fullscreen) 
    {
        // construct a normal window with the specified properties
        window = SDL_CreateWindow(title,
            dimensions.x, dimensions.y,
            dimensions.w, dimensions.h,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        );
    } 
    else 
    {
        // if not construct a fullscreen window with the specified properties
        window = SDL_CreateWindow(title,
            dimensions.x, dimensions.y,
            maxWidth, maxHeight,
            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
        );
    }

    if (icon != "")
    {
        SDL_Surface * windowIcon = SDL_LoadBMP(icon.c_str());
        SDL_SetWindowIcon(window, windowIcon);
        SDL_FreeSurface(windowIcon);
    }

    // The create the GL context
    windowContext = SDL_GL_CreateContext(window);

    // and make it the current context for the window we just created
    SDL_GL_MakeCurrent(window, windowContext);
    vsync = (SDL_GL_SetSwapInterval(1) == NULL);

    if (!vsync)
    {
        MessageBox(NULL, "Vsync is not available", "Warning", MB_OK);
    }
}

bool Window::vsyncEnabled()
{
    return vsync;
}

// Makes a window fullscreen and also overrides certain properties
void Window::makeFullscreen() 
{
    // First we need to get the current display properties
    SDL_DisplayMode current;
    // the iterate through the possible list of displays
    for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
    {
        // and when we arrive at the first display
        int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

        // if we succeed in grabbing the values
        if (should_be_zero == 0) {
            // set the width and height to be equal to the fullscreen
            dimensions.h = current.h;
            dimensions.w = current.w;
            dimensions.x = 0;
            dimensions.y = 0;
            break;
        }
    }

    // and instruct the create method to now create a fullscreen window
    fullscreen = true;
}

// just returns a pointer to the SDL_Window object
SDL_Window * Window::getWindow()
{
    return window;
}