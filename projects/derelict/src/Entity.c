//
// Created by johan on 2026-10-02.
//

#include "Entity.h"
#include "Globals.h"
#include "Log.h"
#include "../res/resources.h"

static const ff32 PLAYER_SPEED = FF32(3.2f);

static void updatePlayer(Entity *player);
static void setNewXPos(Entity *entity);
static void setNewYPos(Entity *entity);
static bool isSolidCol(s16 tx, s16 tyTop, s16 tyBottom);
static bool isSolidRow(s16 ty, s16 txLeft, s16 txRight);

void Entity_init(
    Entity *entity,
    const EntityType type,
    const SpriteDefinition *sprDef,
    const u16 sprAttr,
    const s16 x,
    const s16 y,
    const u8 boundXIndent,
    const u8 boundYIndent,
    const u8 w,
    const u8 h,
    const ff32 vel
) {
    entity->type = type;
    entity->pos = (Vect2D_ff32){.x = FF32(x), .y = FF32(y)};
    entity->vel = (Vect2D_ff32){.x = vel, .y = 0};
    entity->spr = SPR_addSprite(sprDef, x, y, sprAttr);
    entity->indentX = boundXIndent;
    entity->indentY = boundYIndent;
    entity->w = w;
    entity->h = h;
    entity->anim = ENTITY_ANIM_IDLE;
    entity->jumpVel = FF32(8.695f);
    LOG_info("Entity created");
}

void Entity_update(Entity *entity) {
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
    SPR_setPosition(
        entity->spr,
        (s16) (FF32_toInt(entity->pos.x) - entity->indentX - camera.x),
        (s16) (FF32_toInt(entity->pos.y) - entity->indentY - camera.y)
    );
    SPR_setAnim(entity->spr, entity->anim);
}

void Entity_destroy(const Entity *const entity) {
    SPR_releaseSprite(entity->spr);
}

static void updatePlayer(Entity *player) {
    Controller_getState(JOY_1, &controller);
    // Check X
    player->vel.x = 0;
    player->vel.y += GRAVITY;
    if (player->vel.y > MAX_GRAVITY) {
        player->vel.y = MAX_GRAVITY;
    }


    player->anim = ENTITY_ANIM_IDLE;
    if (controller.left) {
        player->vel.x = -PLAYER_SPEED;
        SPR_setHFlip(player->spr, TRUE);
    } else if (controller.right) {
        player->vel.x = PLAYER_SPEED;
        SPR_setHFlip(player->spr, FALSE);
    }

    // Did we move on X axis?
    if (ENTITY_MOVED(player->vel.x)) {
        player->anim = ENTITY_ANIM_WALK;
        setNewXPos(player);
    }

    // Check Y
    if (controller.a && player->onGround) {
        XGM2_playPCM(sfx_jump, sizeof(sfx_jump), SOUND_PCM_CH2);
        player->vel.y -= player->jumpVel;
        player->onGround = false;
    }
    if (controller.up) {
        player->vel.y = -PLAYER_SPEED;
    }

    // Did we move on Y axis?
    if (ENTITY_MOVED(player->vel.y)) {
        setNewYPos(player);
    }

    // Safeguard
    if (player->pos.y > FF32(MAP_HEIGHT)) {
        player->pos.y = FF32(SCREEN_WIDTH >> 1);
        player->pos.x = FF32(200);
        XGM2_playPCM(sfx_die, sizeof(sfx_die), SOUND_PCM_CH2);
    }
}

static void setNewXPos(Entity *entity) {
    // Move to new X
    entity->pos.x += entity->vel.x;
    entity->anim = ENTITY_ANIM_WALK;

    // Convert to tile positions
    const s16 x = FF32_toInt(entity->pos.x);
    const s16 y = FF32_toInt(entity->pos.y);
    const s16 tyTop = y >> 4;
    const s16 tyBottom = (y + entity->h - 1) >> 4;

    // Check for and resolve potential collision on the X axis
    if (ENTITY_HEADING_RIGHT(entity->vel.x)) {
        // Resolve collision on the right edge
        const s16 txRightEdge = (x + entity->w - 1) >> 4;
        if (isSolidCol(txRightEdge, tyTop, tyBottom)) {
            entity->pos.x = FF32((txRightEdge << 4) - entity->w);
            entity->anim = ENTITY_ANIM_IDLE;
        }
    } else {
        // Resolve collision on the LEFT edge
        const s16 txLeftEdge = x >> 4;
        if (isSolidCol(txLeftEdge, tyTop, tyBottom)) {
            entity->pos.x = FF32((txLeftEdge + 1) << 4);
            entity->anim = ENTITY_ANIM_IDLE;
        }
    }
}

static void setNewYPos(Entity *entity) {
    // Move to new X
    entity->pos.y += entity->vel.y;

    // Convert to tile positions
    const s16 x = FF32_toInt(entity->pos.x);
    const s16 y = FF32_toInt(entity->pos.y);
    const s16 txLeft = x >> 4;
    const s16 txRight = (x + entity->w - 1) >> 4;

    // Check for and resolve potential collision on the X axis
    if (ENTITY_HEADING_DOWN(entity->vel.y)) {
        // Resolve collision on the BOTTOM edge
        const s16 tyBottom = (y + entity->h - 1) >> 4;
        if (isSolidRow(tyBottom, txLeft, txRight)) {
            entity->pos.y = FF32((tyBottom << 4) - entity->h);
            entity->onGround = true;
            entity->vel.y = 0;
        } else {
            entity->onGround = false;
        }
    } else {
        // Resolve collision on the TOP edge
        const s16 tyTopEdge = y >> 4;
        if (isSolidRow(tyTopEdge, txLeft, txRight)) {
            entity->pos.y = FF32((tyTopEdge + 1) << 4);
        }
    }
}

static bool isSolidCol(const s16 tx, const s16 tyTop, const s16 tyBottom) {
    for (s16 ty = tyTop; ty <= tyBottom; ty++) {
        if (ty*2 > 0 && ty*2 < tilemap_collision.h && tx*2 > 0 && tx*2 < tilemap_collision.w) {
            if (tilemap_collision.tilemap[((tilemap_collision.w*2) * ty) + tx*2]) {
                return true;
            }
        }
    }
    return false;
}

static bool isSolidRow(const s16 ty, const s16 txLeft, const s16 txRight) {
    for (s16 tx = txLeft; tx <= txRight; tx++) {
        if (tx*2 > 0 && tx*2 < tilemap_collision.w && ty*2 > 0 && ty*2 < tilemap_collision.h) {
            if (tilemap_collision.tilemap[((tilemap_collision.w*2) * ty) + tx*2]) {
                return true;
            }
        }
    }
    return false;
}
