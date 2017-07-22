//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <iostream>
#include "Place.cc"
#include <math.h>

SDL_Texture* loadTexture(std::string path);

// Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const double PI = 3.14159265;

// global vars
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

int main( int argc, char* args[] ) {
  // init SDL
  {
    bool success = true;
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
      printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
      success = false;
    } else {
      // create window
      gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
      if( gWindow == NULL ) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        success = false;
      } else {
        // create renderer
        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (gRenderer == NULL) {
          printf("Renderer could not init. SDL Error: %s\n", SDL_GetError());
          success = false;
        } else {
          SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        }
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
  // load textures
  SDL_Texture* startTexture = loadTexture( "assets/images/press.bmp" );
  SDL_Texture* dogTexture = loadTexture( "assets/images/dog.bmp" );
  SDL_Texture* forestTexture = loadTexture( "assets/images/forest.bmp" );
  // load music
  Mix_Music *music = Mix_LoadMUS("assets/audio/music.wav");
  // load sound effects
  Mix_Chunk *aahSound = Mix_LoadWAV("assets/audio/aah.wav");

  if (!success) {
    return 0;
  }

  //make places
  Place* dogPlace = new Place(dogTexture, aahSound);
  Place* forestPlace = new Place(forestTexture, NULL);

  dogPlace->places[3] = forestPlace;
  forestPlace->places[1] = dogPlace;

  // setup main loop
  bool quit = false;
  bool playing = false;
  gTexture = startTexture;
  Place* currPlace;
  SDL_Event e;

  // visual effects consts
  double redRate = 0.173836; // per second
  double greenRate = 0.3238972; // per second
  double blueRate = 0.0723382; // per second

  // start music
  Mix_PlayMusic(music, -1);

  // main loop
  int elapsedTicks = SDL_GetTicks();
  int delta;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else {
        // playing loop
        if (playing) {
          if (e.type == SDL_KEYDOWN) {
            Place* nextPlace = NULL;
            if (e.key.keysym.sym == SDLK_UP) {
              nextPlace = currPlace->upAction();
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
              nextPlace = currPlace->rightAction();
            } else if (e.key.keysym.sym == SDLK_DOWN) {
              nextPlace = currPlace->downAction();
            } else if (e.key.keysym.sym == SDLK_LEFT) {
              nextPlace = currPlace->leftAction();
            } else if (e.key.keysym.sym == SDLK_ESCAPE) {
              // deallocate everything and go back to main menu
            }
            if (nextPlace != NULL) {
              currPlace = nextPlace;
              Mix_PlayChannel(-1, currPlace->sound, 0);
            }
            gTexture = currPlace->texture;
          }
        // startscreen loop
        } else {
          if (e.type == SDL_KEYDOWN) {
            currPlace = dogPlace;
            gTexture = currPlace->texture;
            Mix_PlayChannel(-1, currPlace->sound, 0);
            playing = true;
          }
        }
      }
    }

    delta = elapsedTicks - SDL_GetTicks();
    elapsedTicks = SDL_GetTicks();

    // visual effects
    double currRed = (sin(elapsedTicks * (redRate / 1000) * 2 * PI) + 1) * 0.5 * 238;
    double currGreen = (sin(elapsedTicks * (greenRate / 1000) * 2 * PI) + 1) * 0.5 * 235;
    double currBlue = (sin(elapsedTicks * (blueRate / 1000) * 2 * PI) + 1) * 0.5 * 20 + 70;

    // render current texture
    if (SDL_SetTextureColorMod(gTexture, currRed, currGreen, currBlue) < 0) {
      printf("SDL_SetTextureColorMod failed. SDL_Error %s\n", SDL_GetError());
    }
    
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
    SDL_RenderPresent(gRenderer);
  }
  // close program
  {
    // stop music
    Mix_HaltMusic();
    // free resources
    SDL_DestroyTexture(gTexture);
    Mix_FreeChunk(aahSound);
    Mix_FreeMusic(music);

    gTexture = NULL;
    aahSound = NULL;
    music = NULL;

    // clean up window
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    gWindow = NULL;
    gRenderer = NULL;

    // quit sdl
    Mix_Quit();
    SDL_Quit();
  }
  return 0;
}

// loadTexture returns SDL_Texture from bmp file at path.
SDL_Texture* loadTexture( std::string path ) {
  // eventual return texture
  SDL_Texture* texture = NULL;
  // load bmp at specified path
  SDL_Surface* surface = SDL_LoadBMP( path.c_str() );
  if (surface == NULL) {
    printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
  } else {
    // create texture from surface
    texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    if (texture == NULL) {
      printf("Could not create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    SDL_FreeSurface(surface);
  }
  return texture;
}
