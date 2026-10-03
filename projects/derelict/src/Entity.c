//
// Created by johan on 2026-10-02.
//

#include "Entity.h"

#include "Collision.h"
#include "Globals.h"

static void updatePlayer(Entity *player);
static void decYPos(Entity *p);
static void incYPos(Entity *p);
static void decXPos(Entity *p);
static void incXPos(Entity *p);

void Entity_init(
    Entity *entity,
    const EntityType type,
    const SpriteDefinition *sprDef,
    const u16 sprAttr,
    const s16 x,
    const s16 y,
    const CollisionBox hitBox,
    const ff32 vel
) {
    entity->type = type;
    entity->pos = entity->prev = (Vect2D_ff32){.x = FF32_fromInt(x), .y = FF32_fromInt(y)};
    entity->vel = (Vect2D_ff32) {.x = vel, .y = vel};
    entity->hitBox = hitBox;
    entity->sprite = SPR_addSprite(sprDef, x, y, sprAttr);

}

void Entity_update(Entity *entity) {
    entity->prev = entity->pos;
    switch (entity->type) {
        case PLAYER:
            updatePlayer(entity);
            break;
        case NPC:
        case ENEMY:
        case STATIC:
            break;
    }
}

void Entity_render(const Entity *entity) {
    SPR_setPosition(entity->sprite, FF32_toInt(entity->pos.x), FF32_toInt(entity->pos.y));
}

void Entity_destroy(const Entity *const entity) {
    SPR_releaseSprite(entity->sprite);
}

void Entity_collideStatic(Entity *e, const CollisionBox *box) {
    const CollisionBox curr = Entity_worldBox(e, e->pos);
    const CollisionBox prev = Entity_worldBox(e, e->prev);
    if (Collision_detect(&curr, box)) {
        const Vect2D_ff32 push = Collision_resolveStatic(&curr, &prev, box);
        e->pos.x += push.x;
        e->pos.y += push.y;
    }
}

static void updatePlayer(Entity *player) {
    if (Controller_changed(&controller)) {
        const Vect2D_ff32 prev = player->pos;
        if (controller.up) {
            decYPos(player);
        } else if (controller.down) {
            incYPos(player);
        }
        if (controller.left) {
            decXPos(player);
            SPR_setHFlip(player->sprite, true);
        } else if (controller.right) {
            incXPos(player);
            SPR_setHFlip(player->sprite, false);
        }
        player->prev = prev;
    }
}

static void decYPos(Entity *p) {
    p->pos.y -= p->vel.y;
}

static void incYPos(Entity *p) {
    p->pos.y += p->vel.y;
}

static void decXPos(Entity *p) {
    p->pos.x -= p->vel.x;
}

static void incXPos(Entity *p) {
    p->pos.x += p->vel.x;
}