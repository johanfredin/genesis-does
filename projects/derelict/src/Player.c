#include "Player.h"
#include "Camera.h"
#include "Controller.h"
#include "Game.h"
#include "Grid.h"
#include "Log.h"

// Anim frames
#define ANIM_IDLE 0
#define ANIM_WALK 1

// Jump macros
#define JUMP_H      (64)        // px
#define JUMP_T      16          // frames to apex (held)
#define JUMP_T_MIN  6           // frames to apex (tap)
#define JUMP_BUFFER_FRAMES 6
#define COYOTE_FRAMES 6

// Gravity macros
#define GRAVITY_F   (2.0f * JUMP_H / (JUMP_T * JUMP_T))  // NEW: float version, reused below

static void setNewXPos(Player *player, const TileMap *grid);

static void setNewYPos(Player *player, const TileMap *grid);

static void handleJump(Player *player);

static const ff32 GRAVITY = FF32(GRAVITY_F);
static const ff32 MAX_GRAVITY = FF32(6.5f);
static const ff32 JUMP_VEL = FF32(2.0f * JUMP_H / JUMP_T);
static const ff32 JUMP_VEL_MIN = FF32(GRAVITY_F * JUMP_T_MIN);

//TODO: This will need to be variable
static const ff32 SPEED = FF32(3.2f);

static u8 g_coyote = 0;
static u8 g_jumpBuffer = 0;

static bool onSoftBlock = false;
static ff32 shimmyDown = FF32(8);

void Player_init(
    Player *player,
    const SpriteDefinition *sprDef,
    const u16 sprAttr,
    const Vect2D_s16 pos,
    const u8 boundXIndent,
    const u8 boundYIndent,
    const u8 w,
    const u8 h
) {
    player->spr = SPR_addSprite(sprDef, pos.x, pos.y, sprAttr);
    player->pos = (Vect2D_ff32){.x = FF32(pos.x), .y = FF32(pos.y)};
    player->indentX = boundXIndent;
    player->indentY = boundYIndent;
    player->w = w;
    player->h = h;
    player->anim = PLAYER_ANIM_IDLE;
    player->vel = (Vect2D_ff32){.x = 0, .y = 0};
    player->state = NONE;
    LOG_debug("Player initialized");
}

static bool handleDropOfLedge(Player *player) {
    if (onSoftBlock) {
        if (Controller_wasPressed(&controller, BUTTON_A) && Controller_isHeld(&controller, BUTTON_DOWN)) {
            player->pos.y += shimmyDown;
            g_coyote = 0;
            onSoftBlock = false;
            g_jumpBuffer = 0;
            return true;
        }
    }
    return false;
}

void Player_update(Player *player, const TileMap *grid) {
    // Check X
    player->vel.x = 0;
    player->vel.y += GRAVITY;
    if (player->vel.y > MAX_GRAVITY) {
        player->vel.y = MAX_GRAVITY;
    }


    player->anim = ANIM_IDLE;
    if (Controller_isHeld(&controller, BUTTON_LEFT)) {
        player->vel.x = -SPEED;
        SPR_setHFlip(player->spr, TRUE);
    } else if (Controller_isHeld(&controller, BUTTON_RIGHT)) {
        player->vel.x = SPEED;
        SPR_setHFlip(player->spr, FALSE);
    }

    if (!handleDropOfLedge(player)) {
        handleJump(player);
    }

    // Did we move on Y axis?
    if (PLAYER_MOVED(player->vel.y)) {
        setNewYPos(player, grid);
    }

    // Did we move on X axis?
    if (PLAYER_MOVED(player->vel.x)) {
        setNewXPos(player, grid);
    }

    // Safeguard
    // TODO: Update
    if (player->pos.y > FF32(GRID_TO_PIXELS(grid->h))) {
        player->pos.x = FF32(160);
        player->pos.y = FF32(190);
        player->vel = (Vect2D_ff32){.x = 0, .y = 0};
    }
}

void Player_render(const Player *player) {
    const Vect2D_s16 *camPos = Camera_getPos();
    SPR_setPosition(
        player->spr,
        (s16) (FF32_toInt(player->pos.x) - player->indentX - camPos->x),
        (s16) (FF32_toInt(player->pos.y) - player->indentY - camPos->y)
    );
    SPR_setAnim(player->spr, player->anim);
}

void Player_destroy(const Player *player) {
    SPR_releaseSprite(player->spr);
}

static void handleJump(Player *player) {
    if (Controller_wasReleased(&controller, BUTTON_A) && player->state == AIR) {
        if (player->vel.y < -JUMP_VEL_MIN) {
            player->vel.y = -JUMP_VEL_MIN;
        }
    }

    // Remember a press for a few frames
    if (Controller_wasPressed(&controller, BUTTON_A)) {
        g_jumpBuffer = JUMP_BUFFER_FRAMES;
    } else if (g_jumpBuffer) {
        g_jumpBuffer--;
    }

    if (g_jumpBuffer && g_coyote) {
        // XGM2_playPCM(sfx_jump, sizeof(sfx_jump), SOUND_PCM_CH2);
        // Button may already be released for a buffered tap: give it the short jump
        player->vel.y = Controller_isHeld(&controller, BUTTON_A) ? -JUMP_VEL : -JUMP_VEL_MIN;
        player->state = AIR;
        g_jumpBuffer = 0;
        g_coyote = 0;
    }
}

static void setNewXPos(Player *player, const TileMap *grid) {
    // Move to new X
    player->pos.x += player->vel.x;
    player->anim = ANIM_WALK;

    // Convert to tile positions
    const s16 x = FF32_toInt(player->pos.x);
    const s16 y = FF32_toInt(player->pos.y);
    const s16 tyTop = GRID_TO_TILE_INDEX(y);
    const s16 tyBottom = GRID_TO_TILE_INDEX(y + player->h - 1);

    // Check for and resolve potential collision on the X axis
    if (PLAYER_HEADING_RIGHT(player->vel.x)) {
        // Resolve collision on the right edge
        const s16 txRightEdge = GRID_TO_TILE_INDEX(x + player->w - 1);
        if (Grid_collisionColAt(grid, txRightEdge, tyTop, tyBottom) == GRID_BLOCK_SOLID) {
            player->pos.x = FF32(GRID_TO_PIXELS(txRightEdge) - player->w);
            player->anim = PLAYER_ANIM_IDLE;
        }
    } else {
        // Resolve collision on the LEFT edge
        const s16 txLeftEdge = GRID_TO_TILE_INDEX(x);
        if (Grid_collisionColAt(grid, txLeftEdge, tyTop, tyBottom) == GRID_BLOCK_SOLID) {
            player->pos.x = FF32(GRID_TO_PIXELS(txLeftEdge + 1));
            player->anim = PLAYER_ANIM_IDLE;
        }
    }
}

static void setNewYPos(Player *player, const TileMap *grid) {
    // Move to new X
    player->pos.y += player->vel.y;

    // Convert to tile positions
    const s16 x = FF32_toInt(player->pos.x);
    const s16 y = FF32_toInt(player->pos.y);
    const s16 txLeft = GRID_TO_TILE_INDEX(x);
    const s16 txRight = GRID_TO_TILE_INDEX(x + player->w - 1);

    // Check for and resolve potential collision on the X axis
    if (PLAYER_HEADING_DOWN(player->vel.y)) {
        // Resolve collision on the BOTTOM edge
        const s16 tyBottom = GRID_TO_TILE_INDEX(y + player->h);
        const GridBlock collision = Grid_collisionRowAt(grid, tyBottom, txLeft, txRight);
        if (collision) {

            onSoftBlock = (collision == GRID_BLOCK_SOFT);

            player->pos.y = FF32(GRID_TO_PIXELS(tyBottom) - player->h);
            player->state = ON_GROUND;
            g_coyote = COYOTE_FRAMES;
            player->vel.y = 0;
        } else {
            if (g_coyote) {
                g_coyote--;
            }
            onSoftBlock = false;
            player->state = AIR;
        }
    } else {
        // Resolve collision on the TOP edge
        const s16 tyTopEdge = GRID_TO_TILE_INDEX(y);
        const u16 collision = Grid_collisionRowAt(grid, tyTopEdge, txLeft, txRight);
        // Only resolve on solid block (subject to change)
        if (collision == GRID_BLOCK_SOLID) {
            player->vel.y = 0;
            player->pos.y = FF32(GRID_TO_PIXELS(tyTopEdge + 1));
        }
    }
}
