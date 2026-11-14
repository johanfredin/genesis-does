//
// Created by johan on 2026-11-07.
//

#ifndef GENESIS_DOES_PLAYER_H
#define GENESIS_DOES_PLAYER_H

#include <genesis.h>

#define PLAYER_HEALTH_FULL (100)
#define PLAYER_ANIM_IDLE (0)
#define PLAYER_ANIM_WALK (1)
#define PLAYER_HEADING_LEFT(velX) (velX < 0)
#define PLAYER_HEADING_RIGHT(velX) (velX > 0)
#define PLAYER_HEADING_UP(velY) (velY) < 0
#define PLAYER_HEADING_DOWN(velY) (velY > 0)
#define PLAYER_MOVED(vel) (vel)

typedef enum PlayerState_ :u8 {
    NONE,
    ON_GROUND,
    AIR,
    LADDER
} PlayerState;

typedef struct Player_ {
    Sprite *spr;
    Vect2D_ff32 pos;
    Vect2D_ff32 vel;
    u8 w;
    u8 h;
    u8 indentX;
    u8 indentY;
    u8 anim;
    u8 health;
    PlayerState state;
} Player;

void Player_init(
    Player *player,
    const SpriteDefinition *sprDef,
    u16 sprAttr,
    Vect2D_s16 pos,
    u8 boundXIndent,
    u8 boundYIndent,
    u8 w,
    u8 h
);

void Player_update(Player *player, const TileMap *grid);
void Player_render(const Player *player);
void Player_destroy(const Player *player);

static inline Vect2D_s16 Player_getCenterPos(const Player *player) {
    return (Vect2D_s16){
        .x = (s16)(FF32_toInt(player->pos.x) + (player->w >> 1)),
        .y = (s16)(FF32_toInt(player->pos.y) + (player->h >> 1))
    };
}

#endif //GENESIS_DOES_PLAYER_H
