#ifndef GENESIS_DOES_GAMEOBJECT_H
#define GENESIS_DOES_GAMEOBJECT_H

#include <genesis.h>
#include "Controller.h"

#define ENTITY_ANIM_IDLE 0
#define ENTITY_ANIM_WALK 1
#define ENTITY_HEADING_LEFT(velX) (velX < 0)
#define ENTITY_HEADING_RIGHT(velX) (velX > 0)
#define ENTITY_HEADING_UP(velY) (velY) < 0
#define ENTITY_HEADING_DOWN(velY) (velY > 0)
#define ENTITY_MOVED(vel) (vel)

typedef enum EntityType {
    PLAYER,
    NPC,
    ENEMY,
    STATIC
} EntityType;

typedef struct Entity_ {
    Sprite *spr;
    EntityType type;
    Vect2D_ff32 pos;
    Vect2D_ff32 vel;
    u8 w;
    u8 h;
    u8 indentX;
    u8 indentY;
    u8 anim;
    bool onGround;
    ff32 jumpVel;
} Entity;

void Entity_init(
    Entity *entity,
    EntityType type,
    const SpriteDefinition *sprDef,
    u16 sprAttr,
    s16 x,
    s16 y,
    u8 boundXIndent,
    u8 boundYIndent,
    u8 w,
    u8 h,
    ff32 vel
);
void Entity_update(Entity *entity);
void Entity_render(const Entity *entity);
void Entity_destroy(const Entity *entity);

#endif //GENESIS_DOES_GAMEOBJECT_H
