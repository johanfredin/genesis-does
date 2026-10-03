#include "../res/resources.h"
#include "Controller.h"
#include "Entity.h"
#include "Globals.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224

#define MAP_WIDTH 384
#define MAP_HEIGHT 240

static Vect2D_f16 camera = {0};
static Map *mapBg = nullptr;
static Map *mapFg = nullptr;
static Entity player = {0};

Controller controller = {0};

static void init();

static void updateCamera();

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
    Entity_init(
        &player,
        PLAYER,
        &spr_cat,
        TILE_ATTR(PAL2, 0, 0, 0),
        SCREEN_WIDTH >> 1,
        SCREEN_HEIGHT >> 1,
        (CollisionBox){.x = FF32(2), .y = FF32(2), .w = FF32(2), .h = FF32(0)},
        FF32(6.5)
    );
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    updateCamera();
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
        Controller_getState(JOY_1, &controller);
        Entity_update(&player);

        updateCamera();
        Entity_render(&player);

        SPR_update();
        SYS_doVBlankProcess();
    }
}
