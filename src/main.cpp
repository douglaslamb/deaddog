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
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

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
SDL_Surface* keyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

//Currently displayed image
SDL_Surface* currentSurface = NULL;

SDL_Surface* loadSurface( std::string path ) {
  //The final optimized image
  SDL_Surface* optimizedSurface = NULL;

  //load image at specified path
  SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
  if (loadedSurface == NULL) {
    printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
  } else {
    //Convert surface to screen format
    optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, NULL);
    if (optimizedSurface == NULL) {
      printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }
  return optimizedSurface;
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

      // set default current surface
      currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

      while (!quit) {
        // pull events from event queue and process until empty
        while (SDL_PollEvent(&e) != 0) {
          if (e.type == SDL_QUIT) {
            quit = true;
          } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
              case SDLK_UP:
                currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_UP];
                break;

              case SDLK_DOWN:
                currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                break;

              case SDLK_LEFT:
                currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                break;

              case SDLK_RIGHT:
                currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                break;
              
              default:
                currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                break;
            }
          }

        }
        //Apply the image stretched
        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;
        SDL_BlitScaled(currentSurface, NULL, screenSurface, &stretchRect);
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

  // Loading success flag
  bool success = true;

  // Load default surface
  keyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "key-press-images/press.bmp" );
  if( keyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL ) {
    printf("Failed to load image.");
    success = false;
  }

  keyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface( "key-press-images/up.bmp" );
  if( keyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL ) {
    printf("Failed to load image.");
    success = false;
  }

  keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface( "key-press-images/down.bmp" );
  if( keyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL ) {
    printf("Failed to load image.");
    success = false;
  }

  keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface( "key-press-images/left.bmp" );
  if( keyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL ) {
    printf("Failed to load image.");
    success = false;
  }

  keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface( "key-press-images/right.bmp" );
  if( keyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL ) {
    printf("Failed to load image.");
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
