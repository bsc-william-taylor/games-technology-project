
#pragma once

#include "EngineLayer.h"

class SILENCE_EXPORT Window
{
    int maxHeight;
    int maxWidth;

    bool fullscreen;
    bool vsync;

    SDL_GLContext windowContext;
    SDL_Window * window;
    SDL_Rect dimensions;
    
    std::string title;
    std::string icon;
public:
    Window();
    ~Window();

    bool isFullscreen();
    bool vsyncEnabled();

    SDL_Rect getWindowDimensions();
    SDL_Window * getWindow();

    void resizeWindow(int x, int y, int w, int h);
    void setPosition(int x, int y);
    void setTitle(const char * t);
    void setSize(int w, int h);
    void makeFullscreen();
    void swapBuffers();
    void create();
    void forceFullscreenMode(int, int);
    void forceFullscreenMode();
    void adjustViewport();
    void setWindowIcon(std::string);
    void forceWindowedMode();

    int getMaxHeight();
    int getMaxWidth();
    int getHeight();
    int getWidth();
};