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
#define HEADING_NONE 5

#define PLAYER_W 48
#define PLAYER_H 48

typedef struct Bounds_ {
    f32 x, y;
    f16 w, h;
} Bounds;

typedef struct player_ {
    Sprite *spr;
    u8 heading;
    Vect2D_f32 pos;
    Vect2D_f32 prev;
    Bounds bounds;
} Player;

static void init();
static void update();
static void initPlayer(u16 palette, u16 prio, bool flipV, bool flibH);
static u16 uploadSolidTile(u16 palette, u16 color);
static bool updatePlayer();
static void checkCollision();
static void logPositions();

static Player player = {0};
static Controller controller = {0};
static Bounds box = {0};
static u16 ind = TILE_USER_INDEX;

static constexpr f32 diff = F32(16);

static void init() {
    SPR_init();
    Controller_init();
    initPlayer(PAL2, 0, FALSE, FALSE);
    // Set up and render rect
    box = (Bounds){
        .x = FIX32(80),
        .y = FIX32(80),
        .w = 80,
        .h = 40
    };
    const u16 tile = uploadSolidTile(PAL0, RGB3_3_3_TO_VDPCOLOR(0xFF, 0, 0));
    // Convert to tile dimension
    VDP_fillTileMapRect(BG_A, tile, F32_toInt(box.x >> 3), F32_toInt(box.y >> 3), box.w >> 3, box.h >> 3);

    ind += TILE_USER_INDEX;

    VDP_drawTextBG(BG_A, "Hello Collision!", 1, 1);

    XGM_startPlay(music);
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
        .x = player.pos.x + diff,
        .y = player.pos.y + diff,
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

static void update() {
    if (updatePlayer()) {
        checkCollision();
    }
    SPR_setPosition(player.spr, (s16) F32_toInt(player.pos.x), (s16) F32_toInt(player.pos.y));
    SPR_update();
}

static bool updatePlayer() {
    static const f32 vel = FIX32(4.5f);
    Controller_getState(JOY_1, &controller);
    player.heading = HEADING_NONE;
    player.bounds.x = player.pos.x + diff;
    player.bounds.y = player.pos.y + diff;
    if (Controller_dpadChanged(&controller)) {
        const fix32 x_prev = player.bounds.x;
        const fix32 y_prev = player.bounds.y;

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
        } else if (controller.right) {
            player.pos.x += vel;
            player.bounds.x += vel;
            player.heading = HEADING_RIGHT;
        }

        player.prev.x = x_prev;
        player.prev.y = y_prev;
        return TRUE;
    }
    return FALSE;
}


static bool overlap() {
    const f32 pbX = player.bounds.x;
    const f32 pbY = player.bounds.y;
    const f32 pbW = FIX32(player.bounds.w);
    const f32 pbH = FIX32(player.bounds.h);

    return (pbX < (box.x + FIX32(box.w))) &
           ((pbX + pbW) > box.x) &
           (pbY < (box.y + FIX32(box.h))) &
           ((pbY + pbH) > box.y);
}

static void resolveCollision() {
    const f32 old_x = player.prev.x;
    const f32 old_y = player.prev.y;
    const f32 right = player.bounds.x + F32(player.bounds.w);
    const f32 oldRight = player.prev.x + F32(player.bounds.w);
    const f32 bottom = player.bounds.y + F32(player.bounds.h);
    const f32 oldBottom = player.prev.y + F32(player.bounds.h);
    const f32 rightBottom = box.y + F32(box.h);
    const f32 boxRight = box.x + F32(box.w);

    if (right >= box.x & oldRight <= box.x) {   // From left
        player.pos.x = (box.x - F32(PLAYER_W));
    } else if (player.bounds.x <= boxRight & old_x >= boxRight) { // From right
        player.pos.x = boxRight - diff;
    } if (bottom >= box.y & oldBottom <= box.y) {
        player.pos.y = (box.y - F32(player.bounds.h)) - diff;
    } else if (player.bounds.y <= rightBottom & old_y >= rightBottom) {
        player.pos.y = rightBottom - diff;
    }

}

static void checkCollision() {
    if (overlap()) {
        VDP_drawTextBG(BG_A, "COLLISION", 1, 5);
        resolveCollision();
    } else {
        VDP_clearText(1, 5, 9);
    }
}

static void logPositions() {
    char pos_buf[32] = {0};
    char box_buf[32] = {0};
    char prev_buf[32] = {0};
    sprintf(pos_buf, "x:%ld, y:%ld", F32_toInt(player.pos.x), F32_toInt(player.pos.y));
    sprintf(box_buf, "bx:%ld, by:%ld", F32_toInt(player.bounds.x), F32_toInt(player.bounds.y));
    sprintf(prev_buf, "px:%ld, py:%ld", F32_toInt(player.prev.x), F32_toInt(player.prev.y));
    VDP_drawTextBG(BG_A, pos_buf, 1, 2);
    VDP_drawTextBG(BG_A, box_buf, 1, 3);
    VDP_drawTextBG(BG_A, prev_buf, 1, 4);
}

int main(bool bool_) {
    init();
    while (TRUE) {
        update();
        logPositions();
        SYS_doVBlankProcess();
    }
}
