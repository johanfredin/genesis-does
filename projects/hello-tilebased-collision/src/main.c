// ReSharper disable CppDFAEndlessLoop
#include <genesis.h>

#include "Controller.h"
#include "Grid.h"
#include "../res/resources.h"

#define SCREEN_W 320
#define SCREEN_H 224

#define HEADING_LEFT 0
#define HEADING_RIGHT 1
#define HEADING_UP 2
#define HEADING_DOWN 3
#define HEADING_NONE 5

typedef struct GO_ {
    Sprite *spr;
    Vect2D_ff32 pos;
    Vect2D_ff32 prev;
    u16 w;
    u16 h;
    u16 indentX;
    u16 indentY;
} GameObject;

static void init();
static void update();
static void initPlayer(u16 palette, u16 prio, u16 boundXIndent, u16 boundYIndent, u16 w, u16 h);
static void updatePlayer();
static void logPositions();

static GameObject player = {0};
static Controller controller = {0};
static u16 ind = TILE_USER_INDEX;

static const ff32 vel = FASTFIX32(4.5f);

static bool overlap() {
    const s16 x = FF32_toInt(player.pos.x);
    const s16 y = FF32_toInt(player.pos.y);

    const s16 firstCol = x >> 4;
    const s16 firstRow = y >> 4;
    const s16 lastCol = (x + player.w - 1) >> 4;
    const s16 lastRow = (y + player.h - 1) >> 4;

    for (s16 ty = firstRow; ty <= lastRow; ty++) {
        for (s16 tx = firstCol; tx <= lastCol; tx++) {
            if (Grid_isSolid(tx, ty)) {
                return true;
            }
        }
    }

    return false;
}

static void init() {
    SPR_init();
    Controller_init();
    initPlayer(PAL2, 0, 16, 16, 16, 32);
    Grid_init(ind);
    VDP_drawTextBG(BG_A, "Hello Collision!", 1, 1);
}

static void initPlayer(const u16 palette, const u16 prio, const u16 boundXIndent, const u16 boundYIndent, const u16 w,
                       const u16 h) {
    PAL_setPalette(PAL2, spr_def_juan.palette->data, DMA);
    player.pos = (Vect2D_ff32){
        .x = FASTFIX32((SCREEN_W >> 1) - (spr_def_juan.w >> 1)),
        .y = FASTFIX32((SCREEN_H >> 1) - (spr_def_juan.h >> 1))
    };
    player.prev = player.pos;

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
}

static void update() {
    updatePlayer();
    SPR_setPosition(player.spr, FF32_toInt(player.pos.x) - player.indentX, FF32_toInt(player.pos.y) - player.indentY);
    SPR_update();
}

static void updatePlayer() {
    Controller_getState(JOY_1, &controller);
    player.prev = player.pos;

    // Check X
    if (controller.left) {
        player.pos.x -= vel;
        SPR_setHFlip(player.spr, TRUE);
    } else if (controller.right) {
        player.pos.x += vel;
        SPR_setHFlip(player.spr, FALSE);
    }

    // Check Y
    if (controller.up) {
        player.pos.y -= vel;
    } else if (controller.down) {
        player.pos.y += vel;
    }

    if (overlap()) {
        VDP_drawTextBG(BG_A, "COLLISION", 1, 2);
    } else {
        VDP_clearText(1, 2, 32);
    }
}


static void logPositions() {
    char pos_buf[32] = {0};
    char prev_buf[32] = {0};
    char tile_buf[32] = {0};
    sprintf(pos_buf, "x:%d, y:%d", FF32_toInt(player.pos.x), FF32_toInt(player.pos.y));
    sprintf(prev_buf, "px:%d, py:%d", FF32_toInt(player.prev.x), FF32_toInt(player.prev.y));
    sprintf(tile_buf, "tx:%d, ty:%d", FF32_toInt(player.pos.x) >> 4, FF32_toInt(player.pos.y) >> 4);
    VDP_drawTextBG(BG_A, pos_buf, 1, 2);
    VDP_drawTextBG(BG_A, prev_buf, 1, 4);
    VDP_drawTextBG(BG_A, tile_buf, 1, 6);
}

int main(bool bool_) {
    init();
    while (TRUE) {
        update();
        logPositions();
        SYS_doVBlankProcess();
    }
}
