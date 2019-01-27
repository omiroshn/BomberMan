#include <SDL2/SDL.h>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(int argc, char* args[])
{
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cout << "Could not initialize sdl2: %s\n" << SDL_GetError() << std::endl;
    return 1;
  }

  window = SDL_CreateWindow( "hello_sdl2",
			    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
          
  if (window == NULL)
  {
    std::cout << "Could not create window: %s\n" << SDL_GetError() << std::endl;
    return 1;
  }
  
  screenSurface = SDL_GetWindowSurface(window);
  SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);
  SDL_Delay(10000);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}