//
// Created by johan on 2026-10-02.
//

#ifndef GENESIS_DOES_GAMEOBJECT_H
#define GENESIS_DOES_GAMEOBJECT_H
#include <genesis.h>

#include "Collision.h"
#include "Controller.h"

typedef enum EntityType {
    PLAYER,
    NPC,
    ENEMY,
    STATIC
} EntityType;

//TODO: Look into using union or different structs for different entities since not all of this will be needed for all
typedef struct Entity {
    Vect2D_ff32 pos;
    Vect2D_ff32 prev;
    Vect2D_ff32 vel;
    Sprite *sprite;
    CollisionBox hitBox;
    u8 currFrame;
    u8 hp;
    u8 damage;
    EntityType type;
} Entity;

static inline CollisionBox Entity_worldBox(const Entity *e, const Vect2D_ff32 at) {
    return (CollisionBox){
        .x = at.x + e->hitBox.x,
        .y = at.y + e->hitBox.y,
        .w = e->hitBox.w,
        .h = e->hitBox.h
    };
}

void Entity_init(Entity *entity, EntityType type, const SpriteDefinition *sprDef, u16 sprAttr, s16 x, s16 y, CollisionBox hitBox, ff32 vel);
void Entity_collideStatic(Entity *e, const CollisionBox *box);
void Entity_update(Entity *entity);
void Entity_render(const Entity *entity);
void Entity_destroy(const Entity *entity);

#endif //GENESIS_DOES_GAMEOBJECT_H
