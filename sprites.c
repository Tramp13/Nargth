#include "dreamwarp.h"

void set_sprite(int index, int x, int y, int w, int h) {
  SDL_Rect sprite = {x, y, w, h};
  sprites[index] = sprite;
}

void init_sprites() {
  for (int y = 0; y < 12; y++) {
      for (int x = 0; x < 16; x++) {
          set_sprite((y * 16) + x, Game.tile_size * x, Game.tile_size * y, Game.tile_size, Game.tile_size);
      }
  }
}
