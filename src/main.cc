//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <iostream>
#include "Place.cc"

void close();
SDL_Surface* loadSurface( std::string path);

// Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

// global vars
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* currSurface = NULL;

// loadSurface returns SDL_Surface from bmp file at path.
SDL_Surface* loadSurface( std::string path ) {
  // eventual return surface
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
    SDL_FreeSurface(loadedSurface);
  }
  return optimizedSurface;
}

int main( int argc, char* args[] ) {
  // init SDL
  {
    bool success = true;
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
      printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
      success = false;
    } else {
      // create window
      window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
      if( window == NULL ) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        success = false;
      } else {
        // get window surface
        screenSurface = SDL_GetWindowSurface( window );
      }
      // init sound
      if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize. SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
      }
    }
    if (!success) {
      return 0;
    }
  }

  // load media
  bool success = true;
  // load default surface
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
  Mix_Chunk *aahSound = Mix_LoadWAV("assets/audio/aah.wav");
  if( aahSound == NULL ) {
    printf("Failed to load sound.");
    success = false;
  }

  if (!success) {
    return 0;
  }

  // setup main loop
  bool quit = false;
  bool playing = false;
  currSurface = startSurface;

  // placeholder to process events
  SDL_Event e;

  // main loop
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else {
        // playing loop
        if (playing) {
          if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_RIGHT) {
              currSurface = startSurface;
              playing = false;
            } else if (e.key.keysym.sym == SDLK_LEFT) {
              currSurface = forest;
            }
          }
        // start screen loop
        } else {
          if (e.type == SDL_KEYDOWN) {
            currSurface = dog;
            Mix_PlayChannel(-1, aahSound, 0);
            playing = true;
          }
        }
      }
    }

    // blit currSurface to screenSurface
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
  Mix_Quit();
  SDL_Quit();
}
