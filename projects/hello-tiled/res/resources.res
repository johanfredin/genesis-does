//IMAGE bg_julie "bg_plane.bmp" BEST ALL
//TILESET jungle_tileset "../../../assets/tilemap/ts_jungle2.tsx" BEST ALL
MAP map_jungle "../../../assets/tilemap/jungle2.tmx" layer_fg FAST FAST
PALETTE jungle_palette "../../../assets/tilemap/ts_jungle2.tsx"

OBJECTS spawn_point "../../../assets/tilemap/jungle2.tmx" "spawn" "name:string;x:u16;y:u16;width:u16;height:u16" "TMX_Spawn"
OBJECTS platforms "../../../assets/tilemap/jungle2.tmx" "platform" "x:u16;y:u16;width:u16;height:u16" "TMX_Platform"


IMAGE jungle_bg "../../../assets/img/jungle_bg.png" BEST ALL
SPRITE spr_cat "../../../assets/spr/mega_cat.png" 6 6 FAST 0

XGM music "../../../assets/music/ooze.vgm"