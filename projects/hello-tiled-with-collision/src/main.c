#include <genesis.h>
#include "../res/resources.h"
// #include "string.h"
#include "Controller.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224

#define MAP_WIDTH 384
#define MAP_HEIGHT 240

static Vect2D_f16 camera = {0};

typedef struct Player_ {
    Sprite *spr;
    Vect2D_ff32 pos;
} Player;

static Controller controller = {0};
static Map *mapBg = nullptr;
static Map *mapFg = nullptr;
static Player player = {0};

static void init();
static void updateCamera();
static bool updatePlayer();

static void init() {
    SPR_initEx(36);
    Controller_init();
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();

    // Init bg_map
    static u16 ind = TILE_USER_INDEX;

    // Init map (bg)
    VDP_loadTileSet(&tileset_bg_jungle3, ind, DMA);
    mapBg = MAP_create(&map_bg_jungle3, BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, ind));
    PAL_setPalette(PAL0, palette_bg_jungle3.data, DMA);
    ind += tileset_bg_jungle3.numTile;

    // Init map (fg)
    VDP_loadTileSet(&tileset_fg_jungle3, ind, DMA);
    mapFg = MAP_create(&map_fg_jungle3, BG_A, TILE_ATTR_FULL(PAL1, 0, 0, 0, ind));
    PAL_setPalette(PAL1, palette_fg_jungle3.data, DMA);
    ind += tileset_fg_jungle3.numTile;

    // Init player
    PAL_setPalette(PAL2, spr_cat.palette->data, DMA);
    player.pos.x = FF32(SCREEN_WIDTH >> 1);
    player.pos.y = FF32(SCREEN_HEIGHT >> 1);
    player.spr = SPR_addSprite(&spr_cat, FF32_toInt(player.pos.x), FF32_toInt(player.pos.y), TILE_ATTR(PAL2, 0, 0, 0));

    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    updateCamera();

}

static bool updatePlayer() {
    static const ff32 vel = FF32(4.5f);
    Controller_getState(JOY_1, &controller);

    if (Controller_dpadChanged(&controller)) {
        if (controller.up) {
            player.pos.y -= vel;
        } else if (controller.down) {
            player.pos.y += vel;
        }

        if (controller.left) {
            player.pos.x -= vel;
            SPR_setHFlip(player.spr, true);
        } else if (controller.right) {
            player.pos.x += vel;
            SPR_setHFlip(player.spr, false);
        }
        return TRUE;
    }
    return FALSE;
}


static void updateCamera() {
    const s32 pX = FF32_toInt(player.pos.x);
    const s32 pY = FF32_toInt(player.pos.y);

    camera.x = clamp(pX - (SCREEN_WIDTH >> 1), 0, MAP_WIDTH - SCREEN_WIDTH);
    camera.y = clamp(pY - (SCREEN_HEIGHT >> 1), 0, MAP_HEIGHT - SCREEN_HEIGHT);

    MAP_scrollTo(mapBg, camera.x >> 2, camera.y >> 2);
    MAP_scrollTo(mapFg, camera.x, camera.y);
}

int main(bool b) {
    if (!b) {
        SYS_hardReset();
    }

    init();
    while (TRUE) {
        if (updatePlayer()) {
            updateCamera();
            SPR_setPosition(player.spr, FF32_toInt(player.pos.x) - camera.x, FF32_toInt(player.pos.y) - camera.y);
        }

        // char buf[32] = {0};
        // const s32 px = player.pos.x;
        // const s32 py = player.pos.y;
        // const u16 tileAt = MAP_getTile(map, px >> 3, py >> 3);
        // const u16 metaTileAt = MAP_getMetaTile(map, px >> 4, py >> 4);

        // sprintf(buf, "tile=%d, metaTile=%d", tileAt, metaTileAt);
        // VDP_drawTextBG(BG_A, buf, 5, 5);
        SPR_update();
        SYS_doVBlankProcess();
    }
}
