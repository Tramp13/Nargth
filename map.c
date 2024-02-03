#include "dreamwarp.h"

Map Map_create(int w, int h) {
    Map map;
    map.w = w;
    map.h = h;
    map.tiles = malloc(sizeof(Entity) * (w * h));
    Entity tile;
    tile.active = false;
    for (int i = 0; i < w * h; i++) {
        map.tiles[i] = tile;
    }
    map.being_count = 0;
    return map;
};

Entity *Map_addBeing(Map *map) {
    Entity being = Entity_create();
    map->being_count++;
    map->beings[map->being_count - 1] = being;

    return &map->beings[map->being_count - 1];
}

void Map_free(Map *map) {
    free(map->tiles);
}

void Map_setTile(Map *map, int x, int y, int type, GameS Game) {
    Entity tile;
    tile.x = x * Game.tile_size;
    tile.y = y * Game.tile_size;
    tile.w = Game.tile_size;
    tile.h = Game.tile_size;
    tile.tile_x = x;
    tile.tile_y = y;
    tile.type = type;
    tile.active = true;
    if (type == 0) {
        tile.active = false;
    }
    tile.spr = type;
    map->tiles[(y * map->w) + x] = tile;
}
