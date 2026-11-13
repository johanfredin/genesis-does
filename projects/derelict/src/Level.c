//
// Created by johan on 2026-11-07.
//

#include "Level.h"

#include "Camera.h"
#include "Globals.h"
#include "Log.h"

static Player *player = nullptr;
static const TileMap *collisionBlocks = nullptr;

void Level_init(Level *level, const LevelDefinition *def, u16 *tileIndex, Player *playerRef) {
    // Init map and map tilesets --------------------------------------------
    // TODO: fix so that these comes from TILED
    level->bounds = (Vect2D_u16){
        .x = MAP_WIDTH,
        .y = MAP_HEIGHT
    };
    // TODO: fix so that these comes from TILED
    level->spawn = (Vect2D_u16){
        .x = 170,
        .y = 190
    };

    VDP_loadTileSet(def->tilesetBG, *tileIndex, DMA);
    level->bg = MAP_create(def->mapBg, BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, *tileIndex));
    PAL_setPalette(PAL0, def->paletteBG->data, DMA);

    *tileIndex += def->tilesetBG->numTile;

    VDP_loadTileSet(def->tilesetFG, *tileIndex, DMA);
    level->fg = MAP_create(def->mapFg, BG_A, TILE_ATTR_FULL(PAL1, 0, 0, 0, *tileIndex));
    PAL_setPalette(PAL1, def->paletteFG->data, DMA);

    *tileIndex += def->tilesetFG->numTile;

    collisionBlocks = def->collision;


    // Assign player and update spawn point ---------------------------------
    player = playerRef;
    player->pos = (Vect2D_ff32) {
        .x = FF32(level->spawn.x),
        .y = FF32(level->spawn.y)
    };

    // Init camera ---------------------------------------------------------
    Camera_init(Player_getCenterPos(player), level->bounds);


    // Done
    LOG_info("Level initialized");
}

void Level_render() {
    Player_render(player);
}

void Level_update(Level *level) {
    Controller_poll(&controller, JOY_1);
    Player_update(player);
    Camera_update(Player_getCenterPos(player));

    SPR_update();
    SYS_doVBlankProcess();
}

void Level_destroy(const Level *level) {
    MAP_release(level->bg);
    MAP_release(level->fg);
}
