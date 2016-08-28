
#include "Window.h"

Window::Window()
    : fullscreen(false),
      window(nullptr),
      title(""),
      icon("")
{
    vsync = false;
}

Window::~Window()
{
    if (window != nullptr) 
    {
        SDL_GL_DeleteContext(windowContext);
        SDL_DestroyWindow(window);
    }
}

int Window::getHeight()
{
    return dimensions.h;
}

void Window::setWindowIcon(std::string filename)
{
    icon = filename;
}

int Window::getWidth()
{
    return dimensions.w;
}

void Window::setPosition(int x, int y)
{
    dimensions.x = x;
    dimensions.y = y;
}

void Window::setTitle(const char * t)
{
    title = t;
}

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
    auto w = 0;
    auto h = 0;
    auto x = 0;
    auto y = 0;

    SDL_GetWindowSize(window, &w, &h);
    SDL_GetWindowPosition(window, &x, &y);

    return { x, y, w, h};
}

int Window::getMaxHeight()
{
    return maxHeight;
}

int Window::getMaxWidth()
{
    return maxWidth;
}

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

    if (!fullscreen) 
    {
        window = SDL_CreateWindow(title.c_str(),
            dimensions.x, dimensions.y,
            dimensions.w, dimensions.h,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        );
    } 
    else 
    {
        window = SDL_CreateWindow(title.c_str(),
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

    windowContext = SDL_GL_CreateContext(window);

    SDL_GL_MakeCurrent(window, windowContext);
    vsync = SDL_GL_SetSwapInterval(1) == NULL;

    if (!vsync)
    {
        MessageBox(nullptr, "Vsync is not available", "Warning", MB_OK);
    }
}

bool Window::vsyncEnabled()
{
    return vsync;
}

void Window::makeFullscreen() 
{
    SDL_DisplayMode current;
    for (auto i = 0; i < SDL_GetNumVideoDisplays(); ++i)
    {
        auto should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

        if (should_be_zero == 0)
        {
            dimensions.h = current.h;
            dimensions.w = current.w;
            dimensions.x = 0;
            dimensions.y = 0;
            break;
        }
    }

    fullscreen = true;
}

SDL_Window * Window::getWindow()
{
    return window;
}