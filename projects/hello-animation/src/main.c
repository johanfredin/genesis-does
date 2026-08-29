// ReSharper disable CppDFAEndlessLoop
#include <genesis.h>
#include "../res/resources.h"

#define ANIM_IDLE 0
#define ANIM_WALK 1

int main() {

    SPR_init();
    PAL_setPalette(PAL2, spr_def_juan.palette->data, DMA);

    const u16 sprite_x = (VDP_getScreenWidth() >> 1) - (spr_def_juan.w >> 1);
    const u16 sprite_y = (VDP_getScreenHeight() >> 1) - (spr_def_juan.h >> 1);

    Sprite *player = SPR_addSprite(&spr_def_juan, sprite_x, sprite_y, TILE_ATTR(PAL2, 1, 0, 0));  // NOLINT(*-narrowing-conversions)
    SPR_setAnim(player, ANIM_WALK);

    VDP_drawText("Hello animated sprite!", 1, 1);

    while (TRUE) {
        SPR_update();
        SYS_doVBlankProcess();
    }

}
