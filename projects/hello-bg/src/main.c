#include <genesis.h>
#include "../res/resources.h"

const fix16 vel = FIX16(1.0);


int main() {
	const u16 ind = TILE_USER_INDEX;
	PAL_setPalette(PAL0, bg_julie.palette->data, DMA);
	fix16 bg_x = FIX16(0.5f);
	VDP_drawImageEx(BG_B, &bg_julie, TILE_ATTR_FULL(PAL0, 0, 0, 0, ind), F16_toInt(bg_x), 0, TRUE, TRUE);

	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

	while(TRUE) {
		bg_x -= vel;
		VDP_setHorizontalScroll(BG_B, F16_toInt(bg_x));
		VDP_waitVSync();
	}
}
