/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
  and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <iostream>

struct Place {
  SDL_Surface* surface;
  Place* left;
  Place* right;
  Place* down;
  Place* up;
};

// Frees media and shuts down SDL 
void close();

// Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

// global vars
SDL_Surface* loadSurface( std::string path);
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* currSurface = NULL;

Place* currPlace = NULL;

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
  // Initialize SDL
  {
    bool success = true;
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
    if (!success) {
      return 0;
    }
  }

  // Load media
  bool success = true;
  // Load default surface
  SDL_Surface* startSurface = loadSurface( "assets/images/press.bmp" );
  if( startSurface == NULL ) {
    printf("Failed to load image.");
    success = false;
  }
  SDL_Surface* dog = loadSurface( "assets/images/dog.bmp" );
  if( dog == NULL ) {
    printf("Failed to load image.");
    success = false;
  }
  SDL_Surface* forest = loadSurface( "assets/images/forest.bmp" );
  if( forest == NULL ) {
    printf("Failed to load image.");
    success = false;
  }
  if (!success) {
    return 0;
  }

  // enter main loop
  bool quit = false;
  // set start screen surface
  currSurface = startSurface;

  // event ref to process events
  SDL_Event e;
  // event handler function placeholder
  // switched out depending on game state
  std::function<void (SDL_Event)> handleEvent;
  std::function<void (SDL_Event)> startHandler;
  std::function<void (SDL_Event)> playingHandler;

  // start screen event handler
  startHandler = [&handleEvent, &playingHandler, &dog] (SDL_Event event) { 
    std::cout << "starthandler ran\n";
    if (event.type == SDL_KEYDOWN) {
    std::cout << "starthandler got a keydown\n";
      handleEvent = playingHandler;
      currSurface = dog;
    }
  };

  // playing game event handler
  playingHandler = [&handleEvent, &startHandler, &startSurface] (SDL_Event event) { 
    std::cout << "playinghandler ran\n";
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {
      handleEvent = startHandler;
      currSurface = startSurface;
    }
  };

  handleEvent = startHandler;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else {
        handleEvent(e);
      }
    }
    // blit the currSurface to the screenSurface
    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;
    SDL_BlitScaled(currSurface, NULL, screenSurface, &stretchRect);
    SDL_UpdateWindowSurface(window);
  }
  close();
  return 0;
}

void close() {
  //Deallocate surface
  SDL_FreeSurface(currSurface);
  currSurface = NULL;

  // clean up window
  SDL_DestroyWindow(window);
  window = NULL;

  // quit sdl
  SDL_Quit();
}
