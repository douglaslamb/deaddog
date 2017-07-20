#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <functional>

class Place {
  public:
    SDL_Surface* surface;
    Mix_Chunk* loadSound;
    std::function<Place*()> leftFunc;
    std::function<Place*()> rightFunc;
    std::function<Place*()> upFunc;
    std::function<Place*()> downFunc;
};
