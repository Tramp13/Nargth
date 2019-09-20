#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>

#include "dreamwarp.h"

#define MAX_COLLISIONS 50

const int FRAME_RATE = 60;

struct CollisionList {
    int tile_count;
    Entity *tiles[50];
} collision_list;

void draw_entity(Entity *entity, SDL_Renderer *renderer, SDL_Texture *texture,
                 SDL_Rect *dst) {
    Entity_dst(dst, entity);
    SDL_RenderCopy(renderer, texture, &sprites[entity->spr], dst);
}

void set_pixel(SDL_Surface* surface, int x, int y, int r, int g, int b) {
    unsigned char* pixels = (unsigned char*)surface->pixels;
    pixels[4 * (y * surface->w + x) + 0] = b;
    pixels[4 * (y * surface->w + x) + 1] = g;
    pixels[4 * (y * surface->w + x) + 2] = r;
    pixels[4 * (y * surface->w + x) + 3] = 255;
}

int main(int argc, char ** argv)
{
  srand(time(0));
  int start_ticks;
  Game.up = Game.down = Game.left = Game.right = Game.attack = false;
  Game.tile_size = 32;
  int frame = 0;
  bool cap = true;
  bool quit = false;
  Timer fps = Timer_create();
  Game.window_width = 15 * Game.tile_size;
  printf("\n%d\n", Game.tile_size);
  Game.window_height = 15 * Game.tile_size;
  init_sprites();
  init_camera();
  SDL_Event event;

  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);

  SDL_Window *window = SDL_CreateWindow("Realm of Realms",
  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480,
  480, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Surface *image = IMG_Load("cavesofgallet_tiles_scaled.png");
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,
  image);

  Map map = Map_create(15, 15);

    char map_array[15 * 15];
    int map_index = 0;
    char *buffer = 0;
    long length;
    FILE *f = fopen("untitled.csv", "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    if (buffer) {
        printf("%s", buffer);
        char delimiter[] = ",\n";
        char *ptr = strtok(buffer, delimiter);

        while (ptr != NULL) {
            map_array[map_index] = atoi(ptr);
            map_index++;
            printf("i%d", map_index);
            ptr = strtok(NULL, delimiter);
        }
    }
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 15; x++) {
            Map_setTile(&map, x, y, map_array[(y * 15) + x]);
        }
    }
  
  Entity *u = Map_addBeing(&map);
  u->w = 32;
  u->h = 32,
  u->speed = 8;
  u->vx = 0;
  u->vy = 0;
  u->spr = 72;
  u->x = 48;
  u->y = 48;
  printf("\n%d\n", Game.window_width);

  /*for (int i = 0; i < 100; i++) {
      Entity *chick = Map_addBeing(&map);
      *chick = Chicken_create();
      Entity *chick_tile = Map_findVacantTile(&map);
      chick->x = chick_tile->x;
      chick->y = chick_tile->y;
  }*/

  SDL_Rect dst;

  while (!quit)
  {
    Timer_start(&fps);
    start_ticks = SDL_GetTicks();
    Game.attack = false;
    SDL_PollEvent(&event);

    switch (event.type)
    {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_x:
            cap = !cap;
            break;
          case SDLK_UP:
            Game.up = true;
            break;
          case SDLK_DOWN:
            Game.down = true;
            break;
          case SDLK_LEFT:
            Game.left = true;
            break;
          case SDLK_RIGHT:
            Game.right = true;
            break;
          case SDLK_SPACE:
            Game.attack = true;
            break;
          case SDLK_ESCAPE:
            quit = true;
            break;
        }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
          case SDLK_UP:
            Game.up = false;
            break;
          case SDLK_DOWN:
            Game.down = false;
            break;
          case SDLK_LEFT:
            Game.left = false;
            break;
          case SDLK_RIGHT:
            Game.right = false;
            break;
          case SDLK_ESCAPE:
            quit = true;
            break;
        }
        break;
    }
    u->vx = 0;
    if (u->on_ceiling && u->vy < 0) {
        u->vy = 0;
        u->on_ceiling = false;
    }
    if (Game.up == false && u->vy < 0) {
        u->vy = 0;
    }
    if (u->on_ground == false) {
        u->vy += .3;
    }

    if (Game.up) {
        if (u->on_ground) {
            u->vy = -u->speed;
            u->facing = NORTH;
            u->on_ground = false;
        }
    }
    if (Game.down) {
      u->vy = u->speed;
      u->facing = SOUTH;
    }
    if (Game.left) {
      u->vx = -u->speed;
      u->facing = WEST;
    }
    if (Game.right) {
      u->vx = u->speed;
      u->facing = EAST;
    }
    if (Game.attack) {
        u->action = ATTACK;
    }

    for (int i = 0; i < map.being_count; i++) {
        if (map.beings[i].active == false) {
            continue;
        }
        map.beings[i].update(&map.beings[i]);
        Entity *e = &map.beings[i];
        int tile_id = floor(e->y / Game.tile_size) *
            map.w + floor(e->x / Game.tile_size);
        Entity *e_tile = &map.tiles[tile_id];
        Entity new_e = *e;

        collision_list.tile_count = 0;
        new_e.x = e->x + e->vx;
        new_e.y = e->y + e->vy;

        for (int y = e_tile->tile_y - 2; y <= e_tile->tile_y + 2; y++) {
            for (int x = e_tile->tile_x - 2; x <= e_tile->tile_x + 2; x++) {
                if (y < 0 || x < 0 || y >= map.h || x >= map.w) {
                    continue;
                }

                Entity *tile = &map.tiles[y * map.w + x];
                if (!tile->active) {
                    continue;
                }
                if (tile->type > 0 && collides(&new_e, tile)) {
                    collision_list.tiles[collision_list.tile_count] = tile;
                    collision_list.tile_count++;
                }
            }
        }
        
        if (e->vx != 0) {
            e->x = e->x + e->vx;
            for (int j = 0; j < collision_list.tile_count; j++) {
                while (collides(e, collision_list.tiles[j])) {
                    if (e->vx > 0) {
                        e->x = e->x - 1;
                    } else if (e->vx < 0) {
                        e->x = e->x + 1;
                    }
                }
            }
        }

        if (e->vy != 0) {
            e->y = e->y + e->vy;
            for (int j = 0; j < collision_list.tile_count; j++) {
                while (collides(e, collision_list.tiles[j])) {
                    if (e->vy > 0) {
                        e->y = e->y - 1;
                        e->on_ground = true;
                    } else if (e->vy < 0) {
                        e->y = e->y + 1;
                        e->on_ceiling = true;
                    }
                }
            }
        }
    }
    
    SDL_RenderClear(renderer);
    for (int i = 0; i < (map.w * map.h); i++) {
        draw_entity(&map.tiles[i], renderer, texture, &dst);
    }
    for (int i = 0; i < map.being_count; i++) {
        draw_entity(&map.beings[i], renderer, texture, &dst);
    }
    SDL_RenderPresent(renderer);
    if ((cap == true) && (Timer_get_ticks(&fps) < 1000 / FRAME_RATE)) {
        SDL_Delay((1000 / FRAME_RATE) - Timer_get_ticks(&fps));
    }
  }

  Map_free(&map);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();

  return 0;
}
