//
// Created by johan on 2026-11-13.
//

#include "Grid.h"
#include <genesis.h>


GridBlock Grid_collisionColAt(const TileMap *grid, const s16 x, s16 top, s16 bottom) {
    // Early return if outside bounds
    if (x < 0 || x >= grid->w) {
        return GRID_BLOCK_NONE;
    }

    // Clamp to edges
    if (top < 0) {
        top = 0;
    }
    if (bottom >= grid->h) {
        bottom = (s16)(grid->h - 1);
    }

    const u16 w = grid->w;
    const u16 *tile = &grid->tilemap[(w * top) + x];
    GridBlock result = GRID_BLOCK_NONE;

    // step one row down at a time
    for (s16 ty = top; ty <= bottom; ty++, tile += w) {
        if (*tile == GRID_BLOCK_SOLID) {
            return GRID_BLOCK_SOLID; // Highest priority, stop early
        }
        if (*tile > result) {
            result = *tile; // Prioritize downward and remember result (soft, ladder, etc)
        }
    }
    return result;

}

GridBlock Grid_collisionRowAt(const TileMap *grid, const s16 y, s16 left, s16 right) {
    // Early return if outside bounds
    if (y < 0 || y >= grid->h) {
        return GRID_BLOCK_NONE;
    }

    // Clamp to edges
    if (left < 0) {
        left = 0;
    }
    if (right >= grid->w) {
        right = (s16)(grid->w - 1);
    }

    const u16 w = grid->w;
    const u16 *tile = &grid->tilemap[(w * y) + left];
    GridBlock result = GRID_BLOCK_NONE;

    for (s16 tx = left; tx <= right; tx++, tile++) {
        if (*tile == GRID_BLOCK_SOLID) {
            return GRID_BLOCK_SOLID;
        }
        if (*tile > result) {
            result = *tile;
        }
    }
    return result;
}