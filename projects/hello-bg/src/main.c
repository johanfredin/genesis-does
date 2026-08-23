#include <genesis.h>
#include "../res/resources.h"

const fix16 vel_bg = FIX16(0.66);
const fix16 vel_fg = FIX16(1.0);

int main() {
	fix16 bg_x = FIX16(0);
	fix16 fg_x = FIX16(0);
	u16 ind = TILE_USER_INDEX;

	PAL_setPalette(PAL0, bg_juan.palette->data, DMA);
	VDP_drawImageEx(BG_B, &bg_juan, TILE_ATTR_FULL(PAL0, 0, 0, 0, ind), F16_toInt(bg_x), 0, TRUE, TRUE);
	ind += bg_juan.tileset->numTile;

	PAL_setPalette(PAL1, fg_juan.palette->data, DMA);
	VDP_drawImageEx(BG_A, &fg_juan, TILE_ATTR_FULL(PAL1, 0, 0, 0, ind), F16_toInt(fg_x), 0, FALSE, TRUE);
	ind += bg_juan.tileset->numTile;

	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	while(TRUE) {
		bg_x -= vel_bg;
		fg_x += vel_fg;

		VDP_setHorizontalScroll(BG_B, F16_toInt(bg_x));
		VDP_setHorizontalScroll(BG_A, F16_toInt(fg_x));
		VDP_waitVSync();
	}
}
