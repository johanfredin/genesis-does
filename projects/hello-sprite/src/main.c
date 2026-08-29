// ReSharper disable CppDFAEndlessLoop
#include <genesis.h>
#include "../res/resources.h"


int main() {

    SPR_init();
    PAL_setPalette(PAL2, spr_def_juan.palette->data, DMA);

    const u16 sprite_x = (VDP_getScreenWidth() >> 1) - (spr_def_juan.w >> 1);
    const u16 sprite_y = (VDP_getScreenHeight() >> 1) - (spr_def_juan.h >> 1);

    SPR_addSprite(&spr_def_juan, sprite_x, sprite_y, TILE_ATTR(PAL2, 1, 0, 0));  // NOLINT(*-narrowing-conversions)

    VDP_drawText("Hello stiff sprite!", 1, 1);

    // Static sprite so no need for calling in a loop here
    SPR_update();

    while (TRUE) {
        SYS_doVBlankProcess();
    }

}
