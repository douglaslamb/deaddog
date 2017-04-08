/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window 
bool init(); 
//Loads media 
bool loadMedia(); 
//Frees media and shuts down SDL 
void close();

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

//The surface contained by the window
SDL_Surface* image = NULL;

int main( int argc, char* args[] ) {
  if (!init()) {
    printf("init failed\n");
  } else {
    if (!loadMedia()) {
      printf("Failed to load media\n");
    } else {
      SDL_BlitSurface(image, NULL, screenSurface, NULL);
      SDL_UpdateWindowSurface(window);
      SDL_Delay(10000);
    }
  }
  close();
  return 0;
}

void close() {
  //Deallocate surface
  SDL_FreeSurface(image);
  image = NULL;

  // clean up window
  SDL_DestroyWindow(window);
  window = NULL;

  // quit sdl
  SDL_Quit();
}

bool loadMedia() {
  bool success = true;

  //load image
  image = SDL_LoadBMP( "image.bmp");
  if (image == NULL) {
    printf("Unable to load image. SDL Error: %s", SDL_GetError());
    success = false;
  }
  return success;
}

bool init() {
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    } else {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

        if( window == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        } else {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );
        }
    }
    return success;
}
