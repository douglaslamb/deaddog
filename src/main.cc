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

  // game state variables
  bool quit = false;
  bool playing = false;

  // load media
  bool success = true;
  // load textures
  SDL_Texture* startTexture = loadTexture( "assets/images/press.bmp" );
  SDL_Texture* dogTexture = loadTexture( "assets/images/dog.bmp" );
  SDL_Texture* deadDogTexture = loadTexture( "assets/images/dead-dog.bmp" );
  SDL_Texture* forestTexture = loadTexture( "assets/images/forest.bmp" );
  // load music
  Mix_Music *music = Mix_LoadMUS("assets/audio/music.wav");
  // load sound effects
  Mix_Chunk *aahSound = Mix_LoadWAV("assets/audio/aah.wav");
  Mix_Chunk *dogDieSound = Mix_LoadWAV("assets/audio/dog-die.wav");

  if (!success) {
    return 0;
  }

  //make places
  Place* dogPlace = new Place(dogTexture, aahSound);
  Place* forestPlace = new Place(forestTexture, NULL);

  // link places
  dogPlace->places[3] = forestPlace;
  forestPlace->places[1] = dogPlace;

  // set actions on places
  dogPlace->actions[3] = [&dogPlace, &deadDogTexture, &dogDieSound](){
    dogPlace->texture = deadDogTexture;
    if (Mix_PlayChannel(-1, dogDieSound, 0) == -1) {
      printf("Mix_PlayChannel error: %s\n", Mix_GetError());
    }
    dogPlace->actions[3] = NULL;
    dogPlace->sound = NULL;
  };

  forestPlace->actions[3] = [&quit]() {
    quit = true;
  };

  // setup main loop
  gTexture = startTexture;
  Place* currPlace;
  SDL_Event e;

  // visual effects consts
  double redRate = 0.173836; // per second
  double greenRate = 0.3238972; // per second
  double blueRate = 0.0723382; // per second

  // start music
  if (Mix_PlayMusic(music, -1) == -1) {
    printf("Mix_PlayMusic error: %s\n", Mix_GetError());
  }

  // main loop
  int elapsedTicks = SDL_GetTicks();
  int delta;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
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
            }
            if (nextPlace != NULL) {
              currPlace = nextPlace;
              if (currPlace->sound != NULL) {
                if (Mix_PlayChannel(-1, currPlace->sound, 0) == -1) {
                  printf("Mix_PlayChannel error: %s\n", Mix_GetError());
                }
              }
            }
            gTexture = currPlace->texture;
          }
        // startscreen loop
        } else {
          if (e.type == SDL_KEYDOWN) {
            currPlace = dogPlace;
            gTexture = currPlace->texture;
            if (currPlace->sound != NULL) {
              if (Mix_PlayChannel(-1, currPlace->sound, 0) == -1) {
                printf("Mix_PlayChannel error: %s\n", Mix_GetError());
              }
            }
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
    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(dogTexture);
    SDL_DestroyTexture(deadDogTexture);
    SDL_DestroyTexture(forestTexture);

    gTexture = NULL;
    startTexture = NULL;
    dogTexture = NULL;
    deadDogTexture = NULL;
    forestTexture = NULL;

    Mix_FreeChunk(aahSound);
    Mix_FreeChunk(dogDieSound);
    Mix_FreeMusic(music);

    aahSound = NULL;
    dogDieSound = NULL;
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
