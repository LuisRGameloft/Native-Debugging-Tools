

#include <WindowManager/WindowManager.hpp>
#include <InputManager/InputManager.hpp>

SDL_Window * WindowManager::window = NULL;
SDL_GLContext WindowManager::context = NULL;
bool WindowManager::isClosed = false;
int WindowManager::timestamp = 0;


// It creates a window and its opengl context
// return false if an error ocurrs
bool WindowManager::createWindow(const char* title, WMenum size)
{
    // this time we supouse that the user only calls this function once
    SDL_Init(SDL_INIT_VIDEO);
    int width;
    int height;
    switch(size)
    {
        case BIG   : { width = 1280; height = 720; break; }
        case SMALL : { width = 960;  height = 540; break; }
        case TINY  : { width = 640;  height = 360; break; }
    }

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_OPENGL
    );

    context = SDL_GL_CreateContext(window);
    if(!context) {
        return false;
    }

    return true;
}

// It destroys the window and its opengl context
void WindowManager::destroyWindow()
{
    if(window)
        SDL_DestroyWindow(window);
    if(context)
        SDL_GL_DeleteContext(context);

    // this time we supouse that the user calls this function
    // at the end of the program
    SDL_Quit();
}

// It polls the window's events
void WindowManager::processEvents()
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
      switch (e.type) {
        case SDL_KEYDOWN:
          InputManager::KeyDown(e.key.keysym.sym);
          break;
        case SDL_KEYUP:
          InputManager::KeyUp(e.key.keysym.sym);
          break;
        case SDL_QUIT:
          isClosed = true;
          destroyWindow();
          break;
      }
    }
}

void WindowManager::swapBuffers()
{
    SDL_GL_SwapWindow(window);
}

float WindowManager::getElapsedTime()
{
    int currenttime = SDL_GetTicks();
    int dt = currenttime - timestamp;
    timestamp = currenttime;
    return dt / 1000.0f;
}
