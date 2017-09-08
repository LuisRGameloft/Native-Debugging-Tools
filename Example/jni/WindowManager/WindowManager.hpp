#pragma once
#ifndef _WINDOWMANAGER_WINDOWMANAGER_H_
#define _WINDOWMANAGER_WINDOWMANAGER_H_

#include <SDL2/SDL.h>

class WindowManager
{
    public:
        enum WMenum
        {
            BIG,
            SMALL,
            TINY
        };
    private:
        static SDL_Window *window;
        static SDL_GLContext context;
        static int timestamp;

    public:
        static bool isClosed;
        static bool createWindow(const char* title, WMenum size);
        static void destroyWindow();
        static void swapBuffers();
        static void processEvents();
        static float getElapsedTime();
};

#endif // _WINDOWMANAGER_WINDOWMANAGER_H_
