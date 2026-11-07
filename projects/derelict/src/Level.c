//
// Created by johan on 2026-11-07.
//

#include "Level.h"

#include "Globals.h"

void Level_init(Level *level, const LevelDefinition *def, u16 tileIndex) {
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

    VDP_loadTileSet(def->mapTilesets[0], tileIndex, DMA);
    level->map = MAP_create(def->map, BG_A, TILE_ATTR_FULL(PAL1, 0, 0, 0, tileIndex));
    PAL_setPalette(PAL1, def->mapPalette->data, DMA);
    tileIndex += def->mapTilesets[0]->numTile;
    
}
