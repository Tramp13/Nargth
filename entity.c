#include "dreamwarp.h"

void Entity_update(Entity *self) {}

Entity Entity_create() {
    Entity entity;
    entity.x = 0;
    entity.y = 0;
    entity.tile_x = 0;
    entity.tile_y = 0;
    entity.w = 0;
    entity.h = 0;
    entity.tile_w = 0;
    entity.tile_h = 0;
    entity.speed = 0;
    entity.vx = 0;
    entity.vy = 0;
    entity.spr = 0;
    entity.type = 0;
    entity.active = true;
    entity.action = 0;
    entity.action_duration = -1;
    entity.move_distance = 0;
    entity.facing = 1;
    entity.update = &Entity_update;
    entity.on_ground = false;
    entity.on_ceiling = false;
    
    return entity;
}
