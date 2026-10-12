// ReSharper disable CppDFAEndlessLoop
#include <genesis.h>

#include "Controller.h"
#include "Grid.h"
#include "../res/resources.h"

#define SCREEN_W 320
#define SCREEN_H 224

#define ANIM_IDLE 0
#define ANIM_WALK 1

#define HEADING_LEFT(velX) (velX < 0)
#define HEADING_RIGHT(velX) (velX > 0)
#define HEADING_UP(velY) (velY) < 0
#define HEADING_DOWN(velY) (velY > 0)
#define MOVED(vel) (vel)

static const ff32 GRAVITY = FF32(0.66f);
static const ff32 MAX = FF32(6.5f);
static const ff32 SPEED = FASTFIX32(3.2f);

typedef struct GO_ {
    Sprite *spr;
    Vect2D_ff32 pos;
    Vect2D_ff32 vel;
    u8 w;
    u8 h;
    u8 indentX;
    u8 indentY;
    u8 anim;
    bool onGround;
    ff32 jumpVel;
} GameObject;

static void init();
static void update();
static void initPlayer(u16 palette, u16 prio, u16 boundXIndent, u16 boundYIndent, u16 w, u16 h);
static void updatePlayer();
static void logPositions();

static GameObject player = {0};
static Controller controller = {0};
static u16 ind = TILE_USER_INDEX;

static void init() {
    SPR_init();
    Controller_init();
    initPlayer(PAL2, 0, 16, 16, 16, 32);
    Grid_init(ind);
    VDP_drawTextBG(BG_A, "Hello Collision!", 1, 1);

    // play a funky tune
    XGM2_play(music);
}

static void initPlayer(const u16 palette, const u16 prio, const u16 boundXIndent, const u16 boundYIndent, const u16 w,
                       const u16 h) {
    PAL_setPalette(PAL2, spr_def_juan.palette->data, DMA);
    player.pos = (Vect2D_ff32){
        .x = FASTFIX32(123),
        .y = FASTFIX32(64)
    };

    player.spr = SPR_addSprite(
        &spr_def_juan,
        (s16) ((VDP_getScreenWidth() >> 1) - (spr_def_juan.w >> 1)),
        (s16) ((VDP_getScreenHeight() >> 1) - (spr_def_juan.h >> 1)),
        TILE_ATTR_FULL(palette, prio, 0, 0, ind)
    );
    ind += TILE_USER_INDEX;
    player.indentX = boundXIndent;
    player.indentY = boundYIndent;
    player.w = w;
    player.h = h;
    player.anim = ANIM_IDLE;
    player.onGround = true;
    player.jumpVel = FF32(8.695f);
}

static void update() {
    updatePlayer();
    SPR_setPosition(player.spr, FF32_toInt(player.pos.x) - player.indentX, FF32_toInt(player.pos.y) - player.indentY);
    SPR_setAnim(player.spr, player.anim);
    SPR_update();
}

static bool isSolidCol(const s16 tx, const s16 tyTop, const s16 tyBottom) {
    for (s16 ty = tyTop; ty <= tyBottom; ty++) {
        if (Grid_isSolid(tx, ty)) {
            return true;
        }
    }
    return false;
}

static bool isSolidRow(const s16 ty, const s16 txLeft, const s16 txRight) {
    for (s16 tx = txLeft; tx <= txRight; tx++) {
        if (Grid_isSolid(tx, ty)) {
            return true;
        }
    }
    return false;
}

static void setNewXPos() {
    // Move to new X
    player.pos.x += player.vel.x;
    player.anim = ANIM_WALK;

    // Convert to tile positions
    const s16 x = FF32_toInt(player.pos.x);
    const s16 y = FF32_toInt(player.pos.y);
    const s16 tyTop = y >> 4;
    const s16 tyBottom = (y + player.h - 1) >> 4;

    // Check for and resolve potential collision on the X axis
    if (HEADING_RIGHT(player.vel.x)) {
        // Resolve collision on the right edge
        const s16 txRightEdge = (x + player.w - 1) >> 4;
        if (isSolidCol(txRightEdge, tyTop, tyBottom)) {
            player.pos.x = FF32((txRightEdge << 4) - player.w);
            player.anim = ANIM_IDLE;
        }
    } else {
        // Resolve collision on the LEFT edge
        const s16 txLeftEdge = x >> 4;
        if (isSolidCol(txLeftEdge, tyTop, tyBottom)) {
            player.pos.x = FF32((txLeftEdge + 1) << 4);
            player.anim = ANIM_IDLE;
        }
    }
}

static void setNewYPos() {
    // Move to new X
    player.pos.y += player.vel.y;

    // Convert to tile positions
    const s16 x = FF32_toInt(player.pos.x);
    const s16 y = FF32_toInt(player.pos.y);
    const s16 txLeft = x >> 4;
    const s16 txRight = (x + player.w - 1) >> 4;

    // Check for and resolve potential collision on the X axis
    if (HEADING_DOWN(player.vel.y)) {
        // Resolve collision on the BOTTOM edge
        const s16 tyBottom = (y + player.h - 1) >> 4;
        if (isSolidRow(tyBottom, txLeft, txRight)) {
            player.pos.y = FF32((tyBottom << 4) - player.h);
            player.onGround = true;
            player.vel.y = 0;
        } else {
            player.onGround = false;
        }
    } else {
        // Resolve collision on the TOP edge
        const s16 tyTopEdge = y >> 4;
        if (isSolidRow(tyTopEdge, txLeft, txRight)) {
            player.pos.y = FF32((tyTopEdge + 1) << 4);
        }
    }
}

static void updatePlayer() {
    Controller_getState(JOY_1, &controller);
    // Check X
    player.vel.x = 0;
    player.vel.y += GRAVITY;
    if (player.vel.y > MAX) {
        player.vel.y = MAX;
    }


    player.anim = ANIM_IDLE;
    if (controller.left) {
        player.vel.x = -SPEED;
        SPR_setHFlip(player.spr, TRUE);
    } else if (controller.right) {
        player.vel.x = SPEED;
        SPR_setHFlip(player.spr, FALSE);
    }

    // Did we move on X axis?
    if (MOVED(player.vel.x)) {
        player.anim = ANIM_WALK;
        setNewXPos();
    }

    // Check Y
    if (controller.a && player.onGround) {
        XGM2_playPCM(sfx_jump, sizeof(sfx_jump), SOUND_PCM_CH2);
        player.vel.y -= player.jumpVel;
        player.onGround = false;
    }
    if (controller.up) {
        player.vel.y = -SPEED;
    }

    // Did we move on Y axis?
    if (MOVED(player.vel.y)) {
        setNewYPos();
    }

    // Safeguard
    if (player.pos.y > FF32(SCREEN_H)) {
        player.pos.y = FF32(64);
        player.pos.x = FF32(123);
        XGM2_playPCM(sfx_die, sizeof(sfx_die), SOUND_PCM_CH2);
    }

}


static void logPositions() {
    char pos_buf[32] = {0};
    char vel_buf[32] = {0};
    char stats[32] = {0};
    sprintf(pos_buf, "x:%-3d, y:%-3d", FF32_toInt(player.pos.x), FF32_toInt(player.pos.y));
    sprintf(vel_buf, "vx:%-3d, vy:%-3d", FF32_toInt(player.vel.x), FF32_toInt(player.vel.y));
    sprintf(stats, "on_ground: %-2d", player.onGround);
    VDP_drawTextBG(BG_A, pos_buf, 1, 2);
    VDP_drawTextBG(BG_A, vel_buf, 1, 3);
    if (player.onGround) {
        VDP_drawTextBG(BG_A, stats, 1, 4);
    }
}

int main(bool bool_) {
    init();
    while (TRUE) {
        update();
        logPositions();
        SYS_doVBlankProcess();
    }
}
