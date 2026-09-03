#include <genesis.h>

#include "Controller.h"
#include "../res/resources.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224

#define MAP_WIDTH 512
#define MAP_HEIGHT 256

static Vect2D_f16 camera = {0};

const fix16 vel_bg = FIX16(0.66);
const fix16 vel_fg = FIX16(1.0);

static Sprite *player = NULL;
static Controller controller = {0};


static void init(void) {
    SPR_init();
    JOY_init();

    // Init bg1
    static u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, bg_juan.palette->data, DMA);
    VDP_drawImageEx(BG_B, &bg_juan, TILE_ATTR_FULL(PAL0, 0, 0, 0, ind), 0, 0, FALSE, TRUE);
    ind += bg_juan.tileset->numTile;

    // Init player
    PAL_setPalette(PAL2, spr_cat.palette->data, DMA);
    player = SPR_addSprite(
        &spr_cat,
        (SCREEN_WIDTH >> 1) - (spr_cat.w >> 1),
        (SCREEN_HEIGHT >> 1) - (spr_cat.h >> 1),
        TILE_ATTR(PAL2, 0, 0, 0)
    );

    // Init bg2
    PAL_setPalette(PAL1, fg_juan.palette->data, DMA);
    VDP_drawImageEx(BG_A, &fg_juan, TILE_ATTR_FULL(PAL1, 1, 0, 0, ind), 0, 0, FALSE, TRUE);
    ind += fg_juan.tileset->numTile;

    // Init camera
    camera.x = 0;
    camera.y = 0;

    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
}

static bool updatePlayer(void) {
    static const fix16 vel = FIX16(3.5f);
    Controller_getState(JOY_1, &controller);

    if (Controller_dpadChanged(&controller)) {
        fix16 x = FIX16(SPR_getPositionX(player));
        fix16 y = FIX16(SPR_getPositionY(player));

        if (controller.up) {
            y -= vel;
        } else if (controller.down) {
            y += vel;
        }

        if (controller.left) {
            x -= vel;
            SPR_setHFlip(player, true);
        } else if (controller.right) {
            x += vel;
            SPR_setHFlip(player, false);
        }

        SPR_setPosition(player, F16_toInt(x - camera.x), F16_toInt(y));
        return TRUE;
    }
    return FALSE;
}


static void updateCamera(void) {
    // Calculate camera pos centered on player and clamp to bounds
    camera.x = clamp(
        SPR_getPositionX(player) - (SCREEN_WIDTH >> 1),
        0,
        MAP_WIDTH);

    camera.y = clamp(SPR_getPositionY(player) - (SCREEN_HEIGHT >> 1), 0, MAP_HEIGHT - SCREEN_HEIGHT);

    VDP_setHorizontalScroll(BG_A, -(camera.x + 3));
    VDP_setHorizontalScroll(BG_B, -(camera.x >> 1));
}

int main(bool b) {
    init();
    while (TRUE) {
        if (updatePlayer()) {
            updateCamera();
        }
        SPR_update();
        SYS_doVBlankProcess();
    }
}
