#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include "Place.h"

Place::Place(SDL_Texture* _texture, Mix_Chunk* _sound) {
  texture = _texture;
  sound = _sound;
}

Place* Place::doAction(int direction) {
  if (actions[direction]) {
    // if there is an action lambda do that
    actions[direction]();
    return NULL;
  } else {
    // else just return the place in that
    // direction, even if it's NULL
    return places[direction];
  }
}

Place* Place::upAction() {
  return doAction(0);
}

Place* Place::rightAction() {
  return doAction(1);
}

Place* Place::downAction() {
  return doAction(2);
}

Place* Place::leftAction() {
  return doAction(3);
}
