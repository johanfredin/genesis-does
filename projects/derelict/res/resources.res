TILESET tileset_bg_jungle3 "../../../assets/tilesets/jungle3_bg.tsx" BEST ALL
TILESET tileset_fg_jungle3 "../../../assets/tilesets/jungle3_fg.tsx" BEST ALL
MAP map_bg_jungle3 "../../../assets/tilemap/jungle3_8x8.tmx" layer_b FAST FAST
MAP map_fg_jungle3 "../../../assets/tilemap/jungle3_8x8.tmx" layer_a FAST FAST
PALETTE palette_bg_jungle3 "../../../assets/tilesets/jungle3_bg.tsx"
PALETTE palette_fg_jungle3 "../../../assets/tilesets/jungle3_fg.tsx"
SPRITE spr_cat "../../../assets/spr/cool-cat-filled.png" 6 6 FAST
TILEMAP tilemap_collision "../../../assets/tilemap/jungle3_8x8.tmx" layer_collision NONE

OBJECTS obj_hardblock "../../../assets/tilemap/jungle3_8x8.tmx" "hardblock" "x:u16;y:u16;width:u16;height:u16" "TMX_HardBlock"
OBJECTS obj_platform "../../../assets/tilemap/jungle3_8x8.tmx" "platform" "x:u16;y:u16;width:u16;height:u16" "TMX_Platform"

//XGM music "../../../assets/music/ooze.vgm"