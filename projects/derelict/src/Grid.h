#ifndef GENESIS_DOES_GRID_H
#define GENESIS_DOES_GRID_H

#include <genesis.h>

#define GRID_TILES_PIXELS_RATIO 3
#define GRID_TO_TILE_INDEX(posInPixels) ((posInPixels) >> GRID_TILES_PIXELS_RATIO)
#define GRID_TO_PIXELS(tileIndex) ((tileIndex) << GRID_TILES_PIXELS_RATIO)

typedef enum GridBlock_ : u8 {
    GRID_BLOCK_NONE,
    GRID_BLOCK_SOFT,
    GRID_BLOCK_LADDER,
    GRID_BLOCK_SOLID
} GridBlock;

GridBlock Grid_collisionColAt(const TileMap *grid, s16 x, s16 top, s16 bottom);
GridBlock Grid_collisionRowAt(const TileMap *grid, s16 y, s16 left, s16 right);

#endif //GENESIS_DOES_GRID_H
