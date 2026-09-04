#include <genesis.h>

#include "Controller.h"
#include "../res/resources.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224

#define MAP_WIDTH 960
#define MAP_HEIGHT 480

static Vect2D_f16 camera = {0};

static Sprite *player = NULL;
static Controller controller = {0};
static Map *map = NULL;

static void init(void) {
    SPR_init();
    JOY_init();
    VDP_setScreenWidth320();

    // Init bg1
    static u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, jungle_bg.palette->data, DMA);
    VDP_drawImageEx(BG_B, &jungle_bg, TILE_ATTR_FULL(PAL0, 0, 0, 0, ind), 0, 0, FALSE, TRUE);
    ind += jungle_bg.tileset->numTile;

    // Init map
    VDP_loadTileSet(&jungle_tileset, ind, DMA);
    map = MAP_create(&map_jungle, BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, ind));
    PAL_setPalette(PAL1, jungle_palette.data, DMA);

    // Init player
    PAL_setPalette(PAL2, spr_cat.palette->data, DMA);
    player = SPR_addSprite(
        &spr_cat,
        (SCREEN_WIDTH >> 1) - (spr_cat.w >> 1),
        (SCREEN_HEIGHT >> 1) - (spr_cat.h >> 1),
        TILE_ATTR(PAL2, 0, 0, 0)
    );


    // Init camera
    camera.x = 0;
    camera.y = 0;

    MAP_scrollTo(map, camera.x, camera.y);
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
}

static bool updatePlayer(void) {
    Controller_getState(JOY_1, &controller);

    if (Controller_dpadChanged(&controller)) {
        s16 x = camera.x;//SPR_getPositionX(player);
        s16 y = camera.y;SPR_getPositionY(player);

        if (controller.up) {
            y -= 1;
        } else if (controller.down) {
            y += 1;
        }

        if (controller.left) {
            x -= 1;
            SPR_setHFlip(player, true);
        } else if (controller.right) {
            x += 1;
            SPR_setHFlip(player, false);
        }

        camera.x += x;
        camera.y += y;
        MAP_scrollTo(map, camera.x, camera.y);

        return TRUE;
    }
    return FALSE;
}


static void updateCamera(void) {
    // Calculate camera pos centered on player and clamp to bounds
    // camera.x = clamp(SPR_getPositionX(player) - (SCREEN_WIDTH >> 1), 0, MAP_WIDTH - SCREEN_WIDTH);
    // camera.y = clamp(SPR_getPositionY(player) - (SCREEN_HEIGHT >> 1), 0, MAP_HEIGHT - SCREEN_HEIGHT);

    s16 px = SPR_getPositionX(player);
    // camera.x = 320;
    //
    // MAP_scrollTo(map, camera.x, 0);
    VDP_setHorizontalScroll(BG_B, -(camera.x >> 1));
}

int main(bool b) {
    init();
    while (TRUE) {
        if (updatePlayer()) {
            SPR_update();
            updateCamera();
        }
        SYS_doVBlankProcess();
    }
}
