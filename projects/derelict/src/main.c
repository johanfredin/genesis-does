#include "../res/resources.h"
#include "Controller.h"
#include "Entity.h"
#include "Globals.h"

Vect2D_s16 camera = {0};
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
        170,
        190,
        16,
        16,
        16,
        32,
        FF32(2.2f)
    );

    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    VDP_setWindowVPos(FALSE, 2);

    updateCamera();
    SYS_doVBlankProcess();

    // play a funky tune
    XGM2_play(music);
}

static void updateCamera() {
    camera.x = FF32_toInt(player.pos.x) + (player.w >> 1) - (SCREEN_WIDTH >> 1);
    camera.y = FF32_toInt(player.pos.y) + (player.w >> 1) - (SCREEN_HEIGHT >> 1);

    if (camera.x < 0) {
        camera.x = 0;
    } else if (camera.x >= MAP_WIDTH - SCREEN_WIDTH) {
        camera.x = MAP_WIDTH - SCREEN_WIDTH;
    }
    if (camera.y < 0) {
        camera.y = 0;
    } else if (camera.y >= MAP_HEIGHT - SCREEN_HEIGHT) {
        camera.y = MAP_HEIGHT - SCREEN_HEIGHT;
    }

    MAP_scrollTo(mapBg, camera.x >> 2, camera.y >> 3);
    MAP_scrollTo(mapFg, camera.x, camera.y);
}

int main(bool b) {
    init();
    while (TRUE) {
        Entity_update(&player);
        updateCamera();
        Entity_render(&player);
        VDP_drawTextBGFill(WINDOW, "Julies cat is cool!", 1, 1, 32);

        SPR_update();
        SYS_doVBlankProcess();
    }
}
