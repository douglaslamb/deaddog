/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
  and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <iostream>

enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window 
bool init(); 
//Loads media 
bool loadMedia(); 
//Frees media and shuts down SDL 
void close();

//Loads individual image
SDL_Surface* loadSurface( std::string path);

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

//The image that corresponsds to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

//Currently displayed image
SDL_Surface* currentSurface = NULL;

SDL_Surface* loadSurface( std::string path ) {
  //load image at specified path
  SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
  if (loadedSurface == NULL) {
    printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
  }
  return loadedSurface;
}

int main( int argc, char* args[] ) {
  if (!init()) {
    printf("init failed\n");
  } else {
    if (!loadMedia()) {
      printf("Failed to load media\n");
    } else {
      // Main loop flag
      bool quit = false;
      //Event 
      SDL_Event e;
      while (!quit) {
        // pull events from event queue and process until empty
        while (SDL_PollEvent(&e) != 0) {
          if (e.type == SDL_QUIT) {
            quit = true;
          }
        }
        SDL_BlitSurface(currentSurface, NULL, screenSurface, NULL);
        SDL_UpdateWindowSurface(window);
      }
    }
  }
  close();
  return 0;
}

void close() {
  //Deallocate surface
  SDL_FreeSurface(currentSurface);
  currentSurface = NULL;

  // clean up window
  SDL_DestroyWindow(window);
  window = NULL;

  // quit sdl
  SDL_Quit();
}

bool loadMedia() {

  //Loading success flag
  bool success = true;
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
