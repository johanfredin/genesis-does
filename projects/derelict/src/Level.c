//
// Created by johan on 2026-11-07.
//

#include "Level.h"
#include "Globals.h"
#include "../res/resources.h"

static Player player = {0};
static const TileMap *collisionBlocks = nullptr;

void Level_init(Level *level, const LevelDefinition *def, u16 *tileIndex) {
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

    // Init player ---------------------------------------------------------
    PAL_setPalette(PAL2, spr_cat.palette->data, DMA);
    Player_init(
        &player,
        &spr_cat,
        TILE_ATTR(PAL2, 0, 0, 0),
        (Vect2D_s16) {.x = 170, .y = 190},
        16,
        16,
        16,
        32,
        FF32(2.2f)
    );


    // Init camera ---------------------------------------------------------

    // Init controller -----------------------------------------------------

}

