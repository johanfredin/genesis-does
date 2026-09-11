#include <genesis.h>

#include "string.h"
#include "Controller.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224

#define MAP_WIDTH 1024
#define MAP_HEIGHT 512

#define PLAYER_CENTER_X (player.pos.x - (spr_cat.w >> 1))
#define PLAYER_CENTER_Y (player.pos.y - (spr_cat.h >> 1))

static Vect2D_f16 camera = {0};

typedef struct Player_ {
    Sprite *spr;
    Vect2D_s32 pos;
} Player;

typedef struct TMX_Spawn_ {
    char *name;
    u16 x, y, w, h;
} TMX_Spawn;

typedef struct TMX_Platform_ {
    u16 x, y, w, h;
} TMX_Platform;

// TODO: FUGGLY solution since TMX_spawn is externally defined in resources.h. A proper lib structure will be done later
// or in a more "serious" project
#include "../res/resources.h"

static Controller controller = {0};
static Map *map = NULL;
static Player player = {0};

static void init(void);

static void updateCamera(void);

static bool updatePlayer(void);

static TMX_Spawn spawn = {0};

static void init(void) {
    SPR_init();
    JOY_init();
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();

    // Init bg1
    static u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, jungle_bg.palette->data, DMA);
    VDP_drawImageEx(BG_B, &jungle_bg, TILE_ATTR_FULL(PAL0, 0, 0, 0, ind), 0, 0, FALSE, TRUE);
    ind += jungle_bg.tileset->numTile;

    // Init map
    VDP_loadTileSet(&map_jungle_tileset0, ind, DMA);
    map = MAP_create(&map_jungle, BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, ind));
    PAL_setPalette(PAL1, jungle_palette.data, DMA);

    // Init player
    PAL_setPalette(PAL2, spr_cat.palette->data, DMA);
    player.pos.x = (SCREEN_WIDTH >> 1);
    player.pos.y = (SCREEN_HEIGHT >> 1);
    player.spr = SPR_addSprite(&spr_cat, player.pos.x, player.pos.y, TILE_ATTR(PAL2, 0, 0, 0));

    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    updateCamera();

    for (size_t i = 0; i < 1; i++) {
        spawn = (TMX_Spawn){
            .name = spawn_point[i]->name,
            .x = spawn_point[i]->x,
            .y = spawn_point[i]->y,
            .w = spawn_point[i]->w,
            .h = spawn_point[i]->h,
        };
    }

    KLog("Hellooooooo");
    kprintf("spawn: name:%s, x:%d, y:%d, w:%d, h:%d\n", spawn.name, spawn.x, spawn.y, spawn.w, spawn.h);
    for (size_t i = 0; i < 8; i++) {
        kprintf("platform: x:%d, y:%d, w:%d, h:%d\n", platforms[i]->x, platforms[i]->y, platforms[i]->w, platforms[i]->h);
    }
}

static bool updatePlayer(void) {
    static const fix16 vel = FIX32(4.5f);
    Controller_getState(JOY_1, &controller);

    if (Controller_dpadChanged(&controller)) {
        fix32 x = FIX32(player.pos.x);
        fix32 y = FIX32(player.pos.y);

        if (controller.up) {
            y -= vel;
        } else if (controller.down) {
            y += vel;
        }

        if (controller.left) {
            x -= vel;
            SPR_setHFlip(player.spr, true);
        } else if (controller.right) {
            x += vel;
            SPR_setHFlip(player.spr, false);
        }

        player.pos.x = F32_toInt(x);
        player.pos.y = F32_toInt(y);
        return TRUE;
    }
    return FALSE;
}


static void updateCamera(void) {
    camera.x = clamp(player.pos.x - (SCREEN_WIDTH >> 1), 0, MAP_WIDTH - SCREEN_WIDTH);
    camera.y = clamp(PLAYER_CENTER_Y - (SCREEN_HEIGHT >> 1), 0, MAP_HEIGHT - SCREEN_HEIGHT);

    MAP_scrollTo(map, camera.x, camera.y);
    VDP_setHorizontalScroll(BG_B, 0 - (camera.x >> 1));
    VDP_setVerticalScroll(BG_B, camera.y >> 4);
}

int main(bool b) {
    if (!b) {
        SYS_hardReset();
    }

    init();
    while (TRUE) {
        if (updatePlayer()) {
            updateCamera();
            SPR_setPosition(player.spr, player.pos.x - camera.x, player.pos.y - camera.y);
        }
        SPR_update();
        SYS_doVBlankProcess();
    }
}
