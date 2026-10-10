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
static void uploadSolidTile(u16 palette, u16 color);
static void updatePlayer();
static void logPositions();

static GameObject player = {0};
static Controller controller = {0};
static Box box = {0};
static u16 ind = TILE_USER_INDEX;

static const ff32 vel = FASTFIX32(4.5f);

static bool overlap() {
    const s16 px = FF32_toInt(player.pos.x);
    const s16 py = FF32_toInt(player.pos.y);

    return px < (box.x + box.w) && px + player.w > box.x &&
           py < (box.y + box.h) && py + player.h > box.y;
}

static void resolveXCollision() {
    const s16 prevX = FF32_toInt(player.prev.x);
    const s16 px = FF32_toInt(player.pos.x);
    if (px + player.w >= box.x && prevX + player.w <= box.x) {
        // From left
        player.pos.x = FF32(box.x - player.w);
    } else if (px <= (box.x + box.w) && prevX >= (box.x + box.w)) {
        // From right
        player.pos.x = FF32(box.x + box.w);
    }
}

static void resolveYCollision() {
    const s16 prevY = FF32_toInt(player.prev.y);
    const s16 py = FF32_toInt(player.pos.y);
    if (py + player.h >= box.y && prevY + player.h <= box.y) {
        player.pos.y = FF32(box.y - player.h);
    } else if (py <= box.y + box.h && prevY >= box.y + box.h) {
        player.pos.y = FF32(box.y + box.h);
    }
}

static void init() {
    SPR_init();
    Controller_init();
    initPlayer(PAL2, 0, 16, 16, 16, 32);

    // Set up and render rect
    box = (Box){
        .x = 80,
        .y = 80,
        .w = 80,
        .h = 40
    };
    uploadSolidTile(PAL0, RGB3_3_3_TO_VDPCOLOR(0xFF, 0, 0));
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
    player.indentX = boundXIndent;
    player.indentY = boundYIndent;
    player.w = w;
    player.h = h;
}

static void uploadSolidTile(const u16 palette, const u16 color) {
    const u32 solidTile[8] = {
        0x11111111, 0x11111111, 0x11111111, 0x11111111,
        0x11111111, 0x11111111, 0x11111111, 0x11111111
    };
    VDP_loadTileData(solidTile, ind, 1, CPU);
    PAL_setColor(palette + 1, color);

    // index +1 - matches the tile's pixel data, index 0 stays the shared background slot
    VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(palette, 1, 0, 0, ind), box.x >> 3, box.y >> 3, box.w >> 3, box.h >> 3);
    ind += TILE_USER_INDEX;
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
    if (overlap()) {
        resolveXCollision();
    }

    // Check Y
    if (controller.up) {
        player.pos.y -= vel;
    } else if (controller.down) {
        player.pos.y += vel;
    }
    if (overlap()) {
        resolveYCollision();
    }
}


static void logPositions() {
    char pos_buf[32] = {0};
    char prev_buf[32] = {0};
    char tile_buf[32] = {0};
    sprintf(pos_buf, "x:%d, y:%d", FF32_toInt(player.pos.x), FF32_toInt(player.pos.y));
    sprintf(prev_buf, "px:%d, py:%d", FF32_toInt(player.prev.x), FF32_toInt(player.prev.y));
    sprintf(tile_buf, "tx:%d, ty:%d", FF32_toInt(player.pos.x) >> 3, FF32_toInt(player.pos.y) >> 3);
    VDP_drawTextBG(BG_A, pos_buf, 1, 2);
    VDP_drawTextBG(BG_A, prev_buf, 1, 4);
    VDP_drawTextBG(BG_A, tile_buf, 1, 6);
}

int main(bool bool_){
    init();
    while (TRUE) {
        update();
        logPositions();
        SYS_doVBlankProcess();
    }
}
