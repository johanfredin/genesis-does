//
// Created by johan on 2026-10-02.
//

#include "Entity.h"

#include "Globals.h"
#include "Log.h"
#include "../res/resources.h"

// The amount to shift by given the tile size (8 == shift by 3, 16 by 4 etc)
#define ENTITY_TILE_SHIFT 3
#define ENTITY_TO_TILE_SIZE(n) ((n) >> ENTITY_TILE_SHIFT)
#define ENTITY_FROM_TILE_SIZE(n) ((n) << ENTITY_TILE_SHIFT)

#define BLOCK_NONE 0
#define BLOCK_SOFT 1
#define BLOCK_LADDER 2
#define BLOCK_SOLID 3

static const ff32 PLAYER_SPEED = FF32(3.2f);

static void updatePlayer(Entity *player);

static void setNewXPos(Entity *entity);

static void setNewYPos(Entity *entity);

static u16 isCollisionCol(s16 tx, s16 tyTop, s16 tyBottom);

static u16 isCollisionRow(s16 ty, s16 txLeft, s16 txRight);

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

// static const ff32 GRAVITY = FF32(0.66f);
static const ff32 MAX_GRAVITY = FF32(6.5f);

#define JUMP_H      (32 * 2)   // px
#define JUMP_T      16         // frames to apex (held)
#define JUMP_T_MIN  6          // NEW: frames to apex (tap)

#define GRAVITY_F   (2.0f * JUMP_H / (JUMP_T * JUMP_T))           // NEW: float version, reused below
static const ff32 GRAVITY      = FF32(GRAVITY_F);                // CHANGED: uses GRAVITY_F
static const ff32 JUMP_VEL     = FF32(2.0f * JUMP_H / JUMP_T);
static const ff32 JUMP_VEL_MIN = FF32(GRAVITY_F * JUMP_T_MIN);   // NEW: v = g * t

// REMOVED: startY, maxY

static void handleJump(Entity *player) {
    // REMOVED: jumpVelMin, jumpVelMax

    if (Controller_wasReleased(&controller, BUTTON_A) && player->jumping) {
        if (player->vel.y < -JUMP_VEL_MIN) {        // CHANGED: was -JUMP_VEL
            player->vel.y = -JUMP_VEL_MIN;          // CHANGED: was -JUMP_VEL
        }
        player->jumping = false;
    }

    if (Controller_wasPressed(&controller, BUTTON_A) && player->onGround) {
        XGM2_playPCM(sfx_jump, sizeof(sfx_jump), SOUND_PCM_CH2);
        // REMOVED: startY / maxY assignments
        player->vel.y = -JUMP_VEL;
        player->onGround = false;
        player->jumping = true;
    }
}


static void updatePlayer(Entity *player) {
    // Check X
    player->vel.x = 0;
    player->vel.y += GRAVITY;
    if (player->vel.y > MAX_GRAVITY) {
        player->vel.y = MAX_GRAVITY;
    }


    player->anim = ENTITY_ANIM_IDLE;
    if (Controller_isHeld(&controller, BUTTON_LEFT)) {
        player->vel.x = -PLAYER_SPEED;
        SPR_setHFlip(player->spr, TRUE);
    } else if (Controller_isHeld(&controller, BUTTON_RIGHT)) {
        player->vel.x = PLAYER_SPEED;
        SPR_setHFlip(player->spr, FALSE);
    }

    // Did we move on X axis?
    if (ENTITY_MOVED(player->vel.x)) {
        player->anim = ENTITY_ANIM_WALK;
        setNewXPos(player);
    }

    handleJump(player);


    if (Controller_isHeld(&controller, BUTTON_UP)) {
        player->vel.y = -PLAYER_SPEED;
    }

    // Did we move on Y axis?
    if (ENTITY_MOVED(player->vel.y)) {
        setNewYPos(player);
    }

    // Safeguard
    if (player->pos.y > FF32(MAP_HEIGHT)) {
        player->pos.x = FF32(160);
        player->pos.y = FF32(190);
        player->vel = (Vect2D_ff32) {.x = 0, .y = 0};
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
    const s16 tyTop = ENTITY_TO_TILE_SIZE(y);
    const s16 tyBottom = ENTITY_TO_TILE_SIZE(y + entity->h - 1);

    // Check for and resolve potential collision on the X axis
    if (ENTITY_HEADING_RIGHT(entity->vel.x)) {
        // Resolve collision on the right edge
        const s16 txRightEdge = ENTITY_TO_TILE_SIZE(x + entity->w - 1);
        if (isCollisionCol(txRightEdge, tyTop, tyBottom) == BLOCK_SOLID) {
            entity->pos.x = FF32(ENTITY_FROM_TILE_SIZE(txRightEdge) - entity->w);
            entity->anim = ENTITY_ANIM_IDLE;
        }
    } else {
        // Resolve collision on the LEFT edge
        const s16 txLeftEdge = ENTITY_TO_TILE_SIZE(x);
        if (isCollisionCol(txLeftEdge, tyTop, tyBottom) == BLOCK_SOLID) {
            entity->pos.x = FF32(ENTITY_FROM_TILE_SIZE(txLeftEdge + 1));
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
    const s16 txLeft = ENTITY_TO_TILE_SIZE(x);
    const s16 txRight = ENTITY_TO_TILE_SIZE(x + entity->w - 1);

    // Check for and resolve potential collision on the X axis
    if (ENTITY_HEADING_DOWN(entity->vel.y)) {
        // Resolve collision on the BOTTOM edge
        const s16 tyBottom = ENTITY_TO_TILE_SIZE(y + entity->h);
        const u16 collision = isCollisionRow(tyBottom, txLeft, txRight);
        if (collision) {
            entity->pos.y = FF32(ENTITY_FROM_TILE_SIZE(tyBottom) - entity->h);
            entity->onGround = true;
            entity->jumping = false;
            entity->vel.y = 0;
        } else {
            entity->onGround = false;
        }
    } else {
        // Resolve collision on the TOP edge
        const s16 tyTopEdge = ENTITY_TO_TILE_SIZE(y);
        const u16 collision = isCollisionRow(tyTopEdge, txLeft, txRight);
        // Only resolve on solid block (subject to change)
        if (collision == BLOCK_SOLID) {
            entity->vel.y = 0;
            entity->pos.y = FF32(ENTITY_FROM_TILE_SIZE(tyTopEdge + 1));
        }
    }
}

static u16 isCollisionCol(const s16 tx, s16 tyTop, s16 tyBottom) {
    // Early return if outside bounds
    if (tx < 0 || tx >= tilemap_collision.w) {
        return false;
    }

    // Clamp to edges
    if (tyTop < 0) {
        tyTop = 0;
    }
    if (tyBottom >= tilemap_collision.h) {
        tyBottom = (s16)(tilemap_collision.h - 1);
    }

    const u16 w = tilemap_collision.w;
    const u16 *tile = &tilemap_collision.tilemap[(w * tyTop) + tx];
    u16 result = 0;

    // step one row down at a time
    for (s16 ty = tyTop; ty <= tyBottom; ty++, tile += w) {
        if (*tile == BLOCK_SOLID) {
            return BLOCK_SOLID; // Highest priority, stop early
        }
        if (*tile > result) {
            result = *tile; // Prioritize downward and remember result (soft, ladder, etc)
        }
    }
    return result;

}

static u16 isCollisionRow(const s16 ty, s16 txLeft, s16 txRight) {
    // Early return if outside bounds
    if (ty < 0 || ty >= tilemap_collision.h) {
        return false;
    }

    // Clamp to edges
    if (txLeft < 0) {
        txLeft = 0;
    }
    if (txRight >= tilemap_collision.w) {
        txRight = (s16)(tilemap_collision.w - 1);
    }

    const u16 w = tilemap_collision.w;
    const u16 *tile = &tilemap_collision.tilemap[(w * ty) + txLeft];
    u16 result = 0;

    for (s16 tx = txLeft; tx <= txRight; tx++, tile++) {
        if (*tile == BLOCK_SOLID) {
            return BLOCK_SOLID;
        }
        if (*tile > result) {
            result = *tile;
        }
    }
    return result;
}
