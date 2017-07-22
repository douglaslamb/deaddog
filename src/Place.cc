#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include "Place.h"

Place::Place(SDL_Texture* _texture, Mix_Chunk* _sound) {
  texture = _texture;
  sound = _sound;
}

Place* Place::doAction(int direction) {
  if (actions[direction]) {
    actions[direction]();
    return NULL;
  } else {
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

