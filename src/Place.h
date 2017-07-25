#ifndef PLACE_H
#define PLACE_H

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <functional>

class Place {
  public:
    SDL_Texture* texture;
    Mix_Chunk* sound;
    std::function<void ()> actions[4] = {};
    Place* places[4] = {};

    Place(SDL_Texture* texture, Mix_Chunk* sound);
    Place* upAction();
    Place* rightAction();
    Place* downAction();
    Place* leftAction();
  private:
    Place* doAction(int direction);
};

#endif
