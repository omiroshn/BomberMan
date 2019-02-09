#include "Core.h"
#include <iostream>
#undef main

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(int argc, char* args[])
{
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;

  BM_INIT_LOG();

  BM_ASSERT_ALWAYS(SDL_Init(SDL_INIT_VIDEO) >= 0);
  BM_CAT_LOG(Init, "SDL Initialized");

  window = SDL_CreateWindow( "hello_sdl2",
			    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
          
  BM_ASSERT_ALWAYS(window);
  BM_CAT_LOG(Init, "SDL Window Initialized");
  
  screenSurface = SDL_GetWindowSurface(window);
  SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);
  SDL_Delay(2000);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}