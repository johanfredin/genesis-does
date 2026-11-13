#include "Player.h"
#include "Camera.h"
#include "Log.h"

#define JUMP_H      (64)        // px
#define JUMP_T      16          // frames to apex (held)
#define JUMP_T_MIN  6           // frames to apex (tap)
#define JUMP_BUFFER_FRAMES 6
#define GRAVITY_F   (2.0f * JUMP_H / (JUMP_T * JUMP_T))  // NEW: float version, reused below
#define COYOTE_FRAMES 6

static const ff32 GRAVITY = FF32(GRAVITY_F);
static const ff32 MAX_GRAVITY = FF32(6.5f);
static const ff32 JUMP_VEL = FF32(2.0f * JUMP_H / JUMP_T);
static const ff32 JUMP_VEL_MIN = FF32(GRAVITY_F * JUMP_T_MIN);

static u8 g_coyote = 0;
static u8 g_jumpBuffer = 0;

void Player_init(
    Player *player,
    const SpriteDefinition *sprDef,
    const u16 sprAttr,
    const Vect2D_s16 pos,
    const u8 boundXIndent,
    const u8 boundYIndent,
    const u8 w,
    const u8 h,
    const Vect2D_ff32 vel
) {
    player->spr = SPR_addSprite(sprDef, pos.x, pos.y, sprAttr);
    player->pos = (Vect2D_ff32){.x = FF32(pos.x), .y = FF32(pos.y)};
    player->indentX = boundXIndent;
    player->indentY = boundYIndent;
    player->w = w;
    player->h = h;
    player->anim = 0;
    player->vel = vel;
    LOG_debug("Player initialized");
}

void Player_update(Player *player) {
}

void Player_render(const Player *player) {
    const Vect2D_s16 *camPos = Camera_getPos();
    SPR_setPosition(
        player->spr,
        (s16) (FF32_toInt(player->pos.x) - player->indentX - camPos->x),
        (s16) (FF32_toInt(player->pos.y) - player->indentY - camPos->y)
    );
    SPR_setAnim(player->spr, player->anim);
}

void Player_destroy(const Player *player) {
    SPR_releaseSprite(player->spr);
}

Vect2D_s16 Player_getCenterPos(const Player *player) {
    //TODO: Maybe turn into macro or inline
    return (Vect2D_s16){
        .x = (s16)(FF32_toInt(player->pos.x) + (player->w >> 1)),
        .y = (s16)(FF32_toInt(player->pos.y) + (player->h >> 1))
    };
}
