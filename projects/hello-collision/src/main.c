// ReSharper disable CppDFAEndlessLoop
#include <genesis.h>

#include "Controller.h"
#include "../res/resources.h"

#define SCREEN_W 320
#define SCREEN_H 224

#define HEADING_LEFT 0
#define HEADING_RIGHT 1
#define HEADING_UP 2
#define HEADING_DOWN 3

typedef struct Bounds_ {
    f32 x, y;
    u8 w, h;
} Bounds;

typedef struct player_ {
    Sprite *spr;
    u8 heading;
    Vect2D_f32 pos;
    Vect2D_f32 prev;
    Bounds bounds;
} Player;

static void init(void);

static void update(void);

static void initPlayer(u16 palette, u16 prio, bool flipV, bool flibH);

static u16 uploadSolidTile(u16 palette, u16 color);

static bool updatePlayer(void);

static void checkCollision(void);

static Player player = {0};
static Controller controller = {0};
static Box box = {0};
static u16 ind = TILE_USER_INDEX;;

int main(bool bool_) {
    init();
    while (TRUE) {
        update();
        SYS_doVBlankProcess();
    }
}

static void init(void) {
    SPR_init();
    Controller_init();
    initPlayer(PAL2, 0, FALSE, FALSE);
    // Set up and render rect
    box = (Box){
        .x = 10,
        .y = 10,
        .w = 10,
        .h = 5
    };
    const u16 tile = uploadSolidTile(PAL0, RGB3_3_3_TO_VDPCOLOR(0xFF, 0, 0));
    VDP_fillTileMapRect(BG_A, tile, box.x, box.y, box.w, box.h);

    ind += TILE_USER_INDEX;

    VDP_drawTextBG(BG_A, "Hello Collision!", 1, 1);
    char buf[32] = {0};
    sprintf(buf, "x:%ld, y:%ld", F32_toInt(player.pos.x), F32_toInt(player.pos.y));
    VDP_drawTextBG(BG_A, buf, 1, 5);
}

static void initPlayer(const u16 palette, const u16 prio, const bool flipV, const bool flibH) {
    PAL_setPalette(PAL2, spr_def_juan.palette->data, DMA);
    player.pos = (Vect2D_f32){
        .x = FIX32(SCREEN_W >> 1),
        .y = FIX32(SCREEN_H >> 1)
    };
    player.prev = player.pos;

    player.spr = SPR_addSprite(
        &spr_def_juan,
        (s16) ((VDP_getScreenWidth() >> 1) - (spr_def_juan.w >> 1)),
        (s16) ((VDP_getScreenHeight() >> 1) - (spr_def_juan.h >> 1)),
        TILE_ATTR_FULL(palette, prio, flipV, flibH, ind)
    );
    player.bounds = (Bounds){
        .x = player.pos.x + FIX32(16),
        .y = player.pos.y + FIX32(16),
        .w = 32,
        .h = 32
    };
}

static u16 uploadSolidTile(const u16 palette, const u16 color) {
    const u32 solidTile[8] = {
        0x11111111, 0x11111111, 0x11111111, 0x11111111,
        0x11111111, 0x11111111, 0x11111111, 0x11111111
    };
    VDP_loadTileData(solidTile, ind, 1, CPU);
    PAL_setColor(palette + 1, color);
    // index +1 - matches the tile's pixel data, index 0 stays the shared background slot

    return TILE_ATTR_FULL(palette, 0, 0, 0, ind);
}

static void update(void) {
    if (updatePlayer()) {
        checkCollision();
    }
    SPR_update();
}

static bool updatePlayer(void) {
    static const fix16 vel = FIX32(4.5f);
    Controller_getState(JOY_1, &controller);

    if (Controller_dpadChanged(&controller)) {
        const fix32 x_prev = player.pos.x;
        const fix32 y_prev = player.pos.y;

        if (controller.up) {
            player.pos.y -= vel;
            player.bounds.y -= vel;
            player.heading = HEADING_UP;
        } else if (controller.down) {
            player.pos.y += vel;
            player.bounds.y += vel;
            player.heading = HEADING_DOWN;
        }

        if (controller.left) {
            player.pos.x -= vel;
            player.bounds.x -= vel;
            player.heading = HEADING_LEFT;
            SPR_setHFlip(player.spr, true);
        } else if (controller.right) {
            player.pos.x += vel;
            player.bounds.x += vel;
            player.heading = HEADING_RIGHT;
            SPR_setHFlip(player.spr, false);
        }

        player.prev.x = x_prev;
        player.prev.y = y_prev;
        SPR_setPosition(player.spr, F32_toInt(player.pos.x), F32_toInt(player.pos.y));
        return TRUE;
    }
    return FALSE;
}

static void checkCollision(void) {
}
