/* Keychron V1 ANSI knob: stock Keychron layout plus lighting overlays.
 * Feature list and host protocol: README.md in this directory.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include QMK_KEYBOARD_H
#include <string.h>
#include "keychron_common.h"
#include "keychron_debounce.h"
#include "via.h"
#include "lib/lib8tion/lib8tion.h"
#include "white_balance.h"

uint8_t white_balance_g = WHITE_BALANCE_G;
uint8_t white_balance_b = WHITE_BALANCE_B;

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

#define FN_MAC MO(MAC_FN)
#define FN_WIN MO(WIN_FN)

enum user_keycodes {
    BOUNCE_RPT = QK_USER_0, // types how many bounces each key has had since plug-in
    BOOT_HOLD,              // hold 2 s to enter the bootloader for flashing
    SNAKE_GAME,             // starts or quits Snake
    BLOCKS_GAME,            // starts or quits Blocks
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_82(
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_DEL,             KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, FN_MAC,   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_ansi_82(
        BOOT_HOLD,KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,            UG_TOGG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
        _______,            _______,  _______,  _______,  _______,  BOUNCE_RPT,SNAKE_GAME,BLOCKS_GAME,_______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

    [WIN_BASE] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,             KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  FN_WIN,   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_ansi_82(
        BOOT_HOLD,KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,            UG_TOGG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
        _______,            _______,  _______,  _______,  _______,  BOUNCE_RPT,SNAKE_GAME,BLOCKS_GAME,_______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = { ENCODER_CCW_CW(UG_VALD, UG_VALU) },
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = { ENCODER_CCW_CW(UG_VALD, UG_VALU) },
};
#endif
// clang-format on

#define BOUNCE_FLASH_MS 2000
// A fast real tap and a bounce both open the contact during the press lockout.
// A real tap's release is reported by ~60 ms (30 lockout + 30 defer); a bounce
// leaves the key held past that.
#define BOUNCE_CONFIRM_MS 75
#define BOOT_HOLD_MS 2000
#define HYPER_SHEET_DELAY_MS 300
#define SHOCK_MS 700
#define SENT_FLASH_MS 450
#define RAGE_FULL 16
#define RAGE_FADE_MS 500
// The board can't read the Mac's volume, so it counts knob clicks: macOS moves
// volume in 1/16 steps per key press. Turning fully down or up re-aligns the
// count after volume was changed elsewhere (menu bar, AirPods).
#define VOLUME_STEPS 16
#define VOLUME_SHOW_MS 1500
#define VOLUME_FADE_MS 300
#define VOLUME_SLIDE_MS 150
#define FLAG_SHOW_MS 2500
#define FLAG_FADE_MS 500
#define SIGNAL_DONE_MS 1400
#define SIGNAL_ERROR_MS 900
#define SIGNAL_ATTENTION_MAX_MS 60000
#define DOOM_STROBE_MS 700
#define DOOM_FADE_FROM_MS 6000
#define DOOM_MS 7000

// Host signals arrive as VIA "custom set value" on the custom channel:
// [0x07, 0x00, SIGNAL_MAGIC, signal, args...]. The magic keeps them apart from
// any custom values a VIA/Launcher definition might add on channel 0.
#define SIGNAL_MAGIC 0x4C
enum host_signal {
    SIGNAL_DONE          = 1,
    SIGNAL_ATTENTION     = 2,
    SIGNAL_ERROR         = 3,
    SIGNAL_CLEAR         = 4,
    SIGNAL_WHITE_BALANCE = 5, // args: green gain, blue gain
    SIGNAL_FLAG_TEST     = 6, // Polish and Ukrainian flags side by side
    SIGNAL_DOOM          = 7, // hell mode; the board also plays it on "wtf?" + Enter
};
#define FLAG_TEST_MS 20000

// Timestamps use timer_read32() | 1 so that 0 can mean "not active".
static uint32_t stamp(void) {
    return timer_read32() | 1;
}

static uint16_t bounce_count[MATRIX_ROWS][MATRIX_COLS];
static uint32_t bounce_flash_at[MATRIX_ROWS][MATRIX_COLS];
static uint32_t bounce_open_at[MATRIX_ROWS][MATRIX_COLS];

static uint32_t boot_hold_at;
static uint32_t caps_down_at;
static uint32_t caps_up_at;
static bool     key_since_caps;
static bool     recording;
static uint32_t sent_flash_at;
static uint32_t shock_at;
static uint8_t  shock_x, shock_y;
static bool     shock_rainbow;
static uint8_t  rage_count;
static uint8_t  rage_fade_from;
static uint32_t rage_fade_at;
static uint8_t  signal_kind;
static uint32_t signal_at;
enum flag { FLAG_NONE, FLAG_ENGLISH, FLAG_UKRAINIAN, FLAG_POLISH, FLAG_TEST };
static uint8_t  flag_shown;
static uint32_t flag_at;
static uint8_t  volume_level = VOLUME_STEPS / 2;
static uint16_t volume_slide_from = VOLUME_STEPS / 2 * 256; // in 1/256 steps
static uint32_t volume_changed_at;
static bool     volume_muted;

// Meter position in 1/256 steps, sliding towards volume_level after a change.
static uint16_t volume_position(void) {
    uint16_t target = volume_level * 256;
    if (!volume_changed_at) return target;
    uint32_t t = timer_elapsed32(volume_changed_at);
    if (t >= VOLUME_SLIDE_MS) return target;
    return volume_slide_from + ((int32_t)target - volume_slide_from) * (int32_t)t / VOLUME_SLIDE_MS;
}

static void volume_changed(int8_t delta) {
    volume_slide_from = volume_position();
    if (delta > 0 && volume_level < VOLUME_STEPS) volume_level++;
    if (delta < 0 && volume_level > 0) volume_level--;
    volume_changed_at = stamp();
}

static void record_bounce(uint8_t row, uint8_t col) {
    if (bounce_count[row][col] < UINT16_MAX) bounce_count[row][col]++;
    bounce_flash_at[row][col] = stamp();
}

void debounce_bounce_caught(uint8_t row, uint8_t col, debounce_bounce_kind_t kind) {
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return;
    if (kind == DEBOUNCE_BOUNCE_RELEASE_CANCELLED) {
        record_bounce(row, col);
    } else if (!bounce_open_at[row][col]) {
        bounce_open_at[row][col] = stamp();
    }
}

// --- Bounce report -----------------------------------------------------------

static char keycode_char(uint16_t kc) {
    if (kc >= KC_A && kc <= KC_Z) return 'a' + (kc - KC_A);
    if (kc >= KC_1 && kc <= KC_9) return '1' + (kc - KC_1);
    if (kc == KC_0) return '0';
    return 0;
}

static void send_bounce_report(void) {
    uint8_t layer = get_highest_layer(default_layer_state);
    bool    any   = false;
    send_string("bounces caught since plug-in:");
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            if (!bounce_count[r][c]) continue;
            char name[8];
            char ch = keycode_char(keymap_key_to_keycode(layer, (keypos_t){.row = r, .col = c}));
            if (ch) {
                name[0] = ch;
                name[1] = 0;
            } else {
                name[0] = 'r';
                name[1] = '0' + r;
                name[2] = 'c';
                name[3] = '0' + c / 10;
                name[4] = '0' + c % 10;
                name[5] = 0;
            }
            char     digits[6];
            uint8_t  pos = sizeof(digits) - 1;
            uint16_t n   = bounce_count[r][c];
            digits[pos]  = 0;
            do {
                digits[--pos] = '0' + n % 10;
                n /= 10;
            } while (n);

            send_string(any ? ", " : " ");
            send_string(name);
            send_string(" ");
            send_string(digits + pos);
            any = true;
        }
    }
    if (!any) send_string(" none");
}

// --- "wtf?" + Enter -----------------------------------------------------------
// Matches physical keys: the input source is applied later, on the Mac, so the
// same keys typed on the Ukrainian layout ("цеа?") match too.

enum wtf_token { WTF_OTHER, WTF_W, WTF_T, WTF_F, WTF_QUESTION, WTF_LETTER };
static uint8_t wtf_typed[5]; // the last five keys, newest last

static bool wtf_watch(uint16_t keycode) {
    if (IS_MODIFIER_KEYCODE(keycode)) return false;
    uint8_t mods = get_mods() | get_oneshot_mods();
    if (keycode == KC_BSPC) {
        memmove(wtf_typed + 1, wtf_typed, sizeof(wtf_typed) - 1);
        wtf_typed[0] = WTF_OTHER;
        return false;
    }
    if (keycode == KC_ENT && !(mods & MOD_MASK_SHIFT)) { // Shift+Enter is a newline in chats
        bool hit = wtf_typed[0] != WTF_LETTER && wtf_typed[1] == WTF_W && wtf_typed[2] == WTF_T && wtf_typed[3] == WTF_F && wtf_typed[4] == WTF_QUESTION;
        memset(wtf_typed, WTF_OTHER, sizeof(wtf_typed));
        return hit;
    }
    uint8_t token = WTF_OTHER;
    if (!caps_down_at && !(mods & (MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI))) { // not shortcuts or Hyper
        if (keycode == KC_W) token = WTF_W;
        else if (keycode == KC_T) token = WTF_T;
        else if (keycode == KC_F) token = WTF_F;
        else if (keycode >= KC_A && keycode <= KC_Z) token = WTF_LETTER;
        else if (keycode == KC_SLSH && (mods & MOD_MASK_SHIFT)) token = WTF_QUESTION;
    }
    memmove(wtf_typed, wtf_typed + 1, sizeof(wtf_typed) - 1);
    wtf_typed[sizeof(wtf_typed) - 1] = token;
    return false;
}

// --- Games --------------------------------------------------------------------
// A running game owns the lights and the key presses (see process_record_user).
// When it ends the board blinks red, then shows the score on the number row
// until a key is pressed or the time runs out.

#define GAME_OVER_BLINK_MS 1200
#define GAME_SCORE_MS 6000

// --- Snake (Fn+N) --------------------------------------------------------------
// The field is the keys themselves: a cell is a key with an LED, so wide keys
// make their row shorter. Left/right go to the next key in the row; up/down go
// to the key in the next row nearest to the column where the vertical run
// began. All four edges wrap, so only the snake's own body is fatal.

#define SNAKE_START_LEN 3
#define SNAKE_STEP_MS 240
#define SNAKE_STEP_MIN_MS 120
#define SNAKE_SPEEDUP_MS 4 // per food eaten
#define SNAKE_LIVE_LENGTH_LEVEL 140 // brightness of the length readout during play

enum snake_phase { SNAKE_OFF, SNAKE_PLAYING, SNAKE_PAUSED, SNAKE_OVER };
enum snake_dir { SNAKE_RIGHT, SNAKE_DOWN, SNAKE_LEFT, SNAKE_UP }; // opposite is dir ^ 2

_Static_assert(MATRIX_ROWS <= 16 && MATRIX_COLS <= 16, "a snake cell packs row and column into one byte");
#define SNAKE_CELL(row, col) ((row) << 4 | (col))
#define SNAKE_ROW(cell) ((cell) >> 4)
#define SNAKE_COL(cell) ((cell)&15)

static uint8_t  snake_phase;
static uint32_t snake_phase_at;
static uint32_t snake_step_at;
static uint8_t  snake_body[MATRIX_ROWS * MATRIX_COLS]; // cells, head first
static uint8_t  snake_len;
static uint8_t  snake_dir;
static uint8_t  snake_turns[2]; // queued, so two quick turns both register
static uint8_t  snake_turn_count;
static uint8_t  snake_column_x;
static uint8_t  snake_food;

static uint8_t snake_led(uint8_t cell) {
    return g_led_config.matrix_co[SNAKE_ROW(cell)][SNAKE_COL(cell)];
}

static bool snake_covers(uint8_t cell, uint8_t segments) {
    for (uint8_t i = 0; i < segments; i++) {
        if (snake_body[i] == cell) return true;
    }
    return false;
}

static bool snake_place_food(void) {
    uint8_t empty[MATRIX_ROWS * MATRIX_COLS];
    uint8_t count = 0;
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t cell = SNAKE_CELL(row, col);
            if (snake_led(cell) != NO_LED && !snake_covers(cell, snake_len)) empty[count++] = cell;
        }
    }
    if (!count) return false;
    snake_food = empty[random8() % count];
    return true;
}

static void snake_start(void) {
    random16_add_entropy(timer_read());
    // On the home row, heading right: head on D, then S and A.
    snake_len = SNAKE_START_LEN;
    for (uint8_t i = 0; i < SNAKE_START_LEN; i++) snake_body[i] = SNAKE_CELL(3, SNAKE_START_LEN - i);
    snake_dir        = SNAKE_RIGHT;
    snake_turn_count = 0;
    snake_column_x   = g_led_config.point[snake_led(snake_body[0])].x;
    snake_place_food();
    snake_phase    = SNAKE_PLAYING;
    snake_phase_at = snake_step_at = timer_read32();
}

static void snake_end(void) {
    snake_phase    = SNAKE_OVER;
    snake_phase_at = timer_read32();
}

static void snake_step(void) {
    if (snake_turn_count) {
        snake_dir      = snake_turns[0];
        snake_turns[0] = snake_turns[1];
        snake_turn_count--;
    }
    uint8_t row = SNAKE_ROW(snake_body[0]);
    uint8_t col = SNAKE_COL(snake_body[0]);
    if (snake_dir == SNAKE_LEFT || snake_dir == SNAKE_RIGHT) {
        do {
            col = (col + (snake_dir == SNAKE_RIGHT ? 1 : MATRIX_COLS - 1)) % MATRIX_COLS;
        } while (g_led_config.matrix_co[row][col] == NO_LED);
        snake_column_x = g_led_config.point[g_led_config.matrix_co[row][col]].x;
    } else {
        row              = (row + (snake_dir == SNAKE_DOWN ? 1 : MATRIX_ROWS - 1)) % MATRIX_ROWS;
        uint8_t nearest = UINT8_MAX;
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            uint8_t led = g_led_config.matrix_co[row][c];
            if (led == NO_LED) continue;
            int16_t gap = g_led_config.point[led].x - snake_column_x;
            if (gap < 0) gap = -gap;
            if (gap < nearest) {
                nearest = gap;
                col     = c;
            }
        }
    }
    uint8_t head = SNAKE_CELL(row, col);
    bool    ate  = head == snake_food;
    // Unless the snake grows, its last segment moves away in this same step.
    uint8_t staying = snake_len - (ate ? 0 : 1);
    if (snake_covers(head, staying)) {
        snake_end();
        return;
    }
    memmove(snake_body + 1, snake_body, staying);
    snake_body[0] = head;
    if (ate) {
        snake_len++;
        if (!snake_place_food()) snake_end(); // every key is snake
    }
}

static void snake_turn(uint8_t dir) {
    uint8_t last = snake_turn_count ? snake_turns[snake_turn_count - 1] : snake_dir;
    if (dir == last || dir == (last ^ 2) || snake_turn_count == ARRAY_SIZE(snake_turns)) return;
    snake_turns[snake_turn_count++] = dir;
}

static void snake_key(uint16_t keycode) {
    if (snake_phase == SNAKE_OVER) {
        if (timer_elapsed32(snake_phase_at) < GAME_OVER_BLINK_MS) return;
        if (keycode == KC_SPC) {
            snake_start();
        } else {
            snake_phase = SNAKE_OFF;
        }
        return;
    }
    switch (keycode) {
        case KC_ESC:
            snake_phase = SNAKE_OFF;
            break;
        case KC_SPC:
            snake_phase   = snake_phase == SNAKE_PAUSED ? SNAKE_PLAYING : SNAKE_PAUSED;
            snake_step_at = timer_read32();
            break;
        case KC_H:
        case KC_A:
        case KC_LEFT:
            snake_turn(SNAKE_LEFT);
            break;
        case KC_J:
        case KC_S:
        case KC_DOWN:
            snake_turn(SNAKE_DOWN);
            break;
        case KC_K:
        case KC_W:
        case KC_UP:
            snake_turn(SNAKE_UP);
            break;
        case KC_L:
        case KC_D:
        case KC_RGHT:
            snake_turn(SNAKE_RIGHT);
            break;
    }
}

static void snake_task(void) {
    if (snake_phase == SNAKE_PLAYING) {
        uint16_t faster = (snake_len - SNAKE_START_LEN) * SNAKE_SPEEDUP_MS;
        uint16_t step   = SNAKE_STEP_MS - MIN(faster, SNAKE_STEP_MS - SNAKE_STEP_MIN_MS);
        if (timer_elapsed32(snake_step_at) >= step) {
            snake_step_at = timer_read32();
            snake_step();
        }
    } else if (snake_phase == SNAKE_OVER && timer_elapsed32(snake_phase_at) > GAME_OVER_BLINK_MS + GAME_SCORE_MS) {
        snake_phase = SNAKE_OFF;
    }
}

// --- Blocks (Fn+M) -------------------------------------------------------------
// A falling-blocks game in a well that lies on its side: five lanes (the key
// rows from the F-row to the Shift row) by twelve keys deep, with the floor at
// the left edge. Pieces enter on the right and fall left; a full column of five
// keys clears. Cell (lane, depth) is the depth-th key of that row, so a column
// slants the way the key columns do (F1, 1, Q, A, Z).

#define BLOCKS_LANES 5
#define BLOCKS_DEPTH 12
#define BLOCKS_STEP_MS 600
#define BLOCKS_STEP_MIN_MS 150
#define BLOCKS_SPEEDUP_MS 30 // per column cleared
#define BLOCKS_SOFT_DROP_MS 60
#define BLOCKS_CLEAR_MS 250

enum blocks_phase { BLOCKS_OFF, BLOCKS_FALLING, BLOCKS_CLEARING, BLOCKS_PAUSED, BLOCKS_OVER };
enum blocks_kind { BLOCKS_DOT, BLOCKS_PAIR, BLOCKS_BAR, BLOCKS_CORNER, BLOCKS_KINDS };

typedef struct {
    int8_t lane, depth;
} blocks_offset_t;

static const struct {
    uint8_t         cells;
    blocks_offset_t at[3]; // offsets from the pivot
    uint8_t         hue;
} blocks_shapes[BLOCKS_KINDS] = {
    [BLOCKS_DOT]    = {1, {{0, 0}}, 43},
    [BLOCKS_PAIR]   = {2, {{0, 0}, {1, 0}}, 128},
    [BLOCKS_BAR]    = {3, {{-1, 0}, {0, 0}, {1, 0}}, 213},
    [BLOCKS_CORNER] = {3, {{0, 0}, {1, 0}, {0, -1}}, 85},
};
// Dots are rare because one fills any hole.
static const uint8_t blocks_bag[] = {BLOCKS_BAR, BLOCKS_BAR, BLOCKS_BAR, BLOCKS_CORNER, BLOCKS_CORNER, BLOCKS_CORNER, BLOCKS_PAIR, BLOCKS_PAIR, BLOCKS_DOT};

static uint8_t         blocks_phase;
static uint32_t        blocks_phase_at;
static uint32_t        blocks_step_at;
static uint8_t         blocks_led[BLOCKS_LANES][BLOCKS_DEPTH];
static uint8_t         blocks_well[BLOCKS_LANES][BLOCKS_DEPTH]; // 0 = empty, else kind + 1
static uint8_t         blocks_kind;
static blocks_offset_t blocks_piece[3]; // the falling piece's shape as rotated
static int8_t          blocks_lane, blocks_depth; // its pivot
static bool            blocks_dropping;
static uint8_t         blocks_cleared; // columns cleared: the score

static bool blocks_fits(int8_t lane, int8_t depth, const blocks_offset_t *piece) {
    for (uint8_t i = 0; i < blocks_shapes[blocks_kind].cells; i++) {
        int8_t l = lane + piece[i].lane;
        int8_t d = depth + piece[i].depth;
        if (l < 0 || l >= BLOCKS_LANES || d < 0) return false;
        // Above the rim is open space.
        if (d < BLOCKS_DEPTH && blocks_well[l][d]) return false;
    }
    return true;
}

static bool blocks_column_full(uint8_t depth) {
    for (uint8_t lane = 0; lane < BLOCKS_LANES; lane++) {
        if (!blocks_well[lane][depth]) return false;
    }
    return true;
}

static void blocks_end(void) {
    blocks_phase    = BLOCKS_OVER;
    blocks_phase_at = timer_read32();
}

static void blocks_spawn(void) {
    blocks_kind = blocks_bag[random8() % ARRAY_SIZE(blocks_bag)];
    memcpy(blocks_piece, blocks_shapes[blocks_kind].at, sizeof(blocks_piece));
    blocks_lane     = BLOCKS_LANES / 2;
    blocks_depth    = BLOCKS_DEPTH - 1;
    blocks_dropping = false; // each piece needs its own press
    blocks_step_at  = timer_read32();
    blocks_phase    = BLOCKS_FALLING;
    if (!blocks_fits(blocks_lane, blocks_depth, blocks_piece)) blocks_end();
}

static void blocks_start(void) {
    random16_add_entropy(timer_read());
    memset(blocks_well, 0, sizeof(blocks_well));
    for (uint8_t lane = 0; lane < BLOCKS_LANES; lane++) {
        uint8_t depth = 0;
        for (uint8_t col = 0; col < MATRIX_COLS && depth < BLOCKS_DEPTH; col++) {
            uint8_t led = g_led_config.matrix_co[lane][col];
            if (led != NO_LED) blocks_led[lane][depth++] = led;
        }
    }
    blocks_cleared  = 0;
    blocks_phase_at = timer_read32();
    blocks_spawn();
}

static void blocks_lock(void) {
    for (uint8_t i = 0; i < blocks_shapes[blocks_kind].cells; i++) {
        int8_t depth = blocks_depth + blocks_piece[i].depth;
        if (depth >= BLOCKS_DEPTH) { // came to rest sticking out of the well
            blocks_end();
            return;
        }
        blocks_well[blocks_lane + blocks_piece[i].lane][depth] = blocks_kind + 1;
    }
    for (uint8_t depth = 0; depth < BLOCKS_DEPTH; depth++) {
        if (blocks_column_full(depth)) {
            // Flash the full columns before blocks_collapse removes them.
            blocks_phase    = BLOCKS_CLEARING;
            blocks_phase_at = timer_read32();
            return;
        }
    }
    blocks_spawn();
}

static void blocks_collapse(void) {
    uint8_t to = 0;
    for (uint8_t from = 0; from < BLOCKS_DEPTH; from++) {
        if (blocks_column_full(from)) {
            if (blocks_cleared < 99) blocks_cleared++;
            continue;
        }
        for (uint8_t lane = 0; lane < BLOCKS_LANES; lane++) blocks_well[lane][to] = blocks_well[lane][from];
        to++;
    }
    for (; to < BLOCKS_DEPTH; to++) {
        for (uint8_t lane = 0; lane < BLOCKS_LANES; lane++) blocks_well[lane][to] = 0;
    }
}

static void blocks_rotate(void) {
    blocks_offset_t turned[3];
    for (uint8_t i = 0; i < ARRAY_SIZE(turned); i++) {
        turned[i] = (blocks_offset_t){.lane = blocks_piece[i].depth, .depth = -blocks_piece[i].lane};
    }
    // Against a wall or the stack, the piece may shift one lane to make room.
    static const int8_t nudges[] = {0, 1, -1};
    for (uint8_t i = 0; i < ARRAY_SIZE(nudges); i++) {
        if (blocks_fits(blocks_lane + nudges[i], blocks_depth, turned)) {
            blocks_lane += nudges[i];
            memcpy(blocks_piece, turned, sizeof(blocks_piece));
            return;
        }
    }
}

// The arrows are named for the keyboard lying flat. Turn the keyboard a quarter
// turn anticlockwise and they still point the right way: Left is "down".
static void blocks_key(uint16_t keycode, bool pressed) {
    if (!pressed) {
        if (keycode == KC_LEFT || keycode == KC_H || keycode == KC_A) blocks_dropping = false;
        return;
    }
    if (blocks_phase == BLOCKS_OVER) {
        if (timer_elapsed32(blocks_phase_at) < GAME_OVER_BLINK_MS) return;
        if (keycode == KC_SPC) {
            blocks_start();
        } else {
            blocks_phase = BLOCKS_OFF;
        }
        return;
    }
    if (keycode == KC_ESC) {
        blocks_phase = BLOCKS_OFF;
        return;
    }
    if (keycode == KC_P && blocks_phase != BLOCKS_CLEARING) {
        blocks_phase   = blocks_phase == BLOCKS_PAUSED ? BLOCKS_FALLING : BLOCKS_PAUSED;
        blocks_step_at = timer_read32();
        return;
    }
    if (blocks_phase != BLOCKS_FALLING) return;
    switch (keycode) {
        case KC_UP:
        case KC_K:
        case KC_W:
            if (blocks_fits(blocks_lane - 1, blocks_depth, blocks_piece)) blocks_lane--;
            break;
        case KC_DOWN:
        case KC_J:
        case KC_S:
            if (blocks_fits(blocks_lane + 1, blocks_depth, blocks_piece)) blocks_lane++;
            break;
        case KC_RGHT:
        case KC_L:
        case KC_D:
            blocks_rotate();
            break;
        case KC_LEFT:
        case KC_H:
        case KC_A:
            blocks_dropping = true;
            break;
        case KC_SPC:
            while (blocks_fits(blocks_lane, blocks_depth - 1, blocks_piece)) blocks_depth--;
            blocks_lock();
            break;
    }
}

static void blocks_task(void) {
    switch (blocks_phase) {
        case BLOCKS_FALLING: {
            uint16_t faster = blocks_cleared * BLOCKS_SPEEDUP_MS;
            uint16_t step   = BLOCKS_STEP_MS - MIN(faster, BLOCKS_STEP_MS - BLOCKS_STEP_MIN_MS);
            if (blocks_dropping) step = BLOCKS_SOFT_DROP_MS;
            if (timer_elapsed32(blocks_step_at) < step) break;
            blocks_step_at = timer_read32();
            if (blocks_fits(blocks_lane, blocks_depth - 1, blocks_piece)) {
                blocks_depth--;
            } else {
                blocks_lock();
            }
        } break;
        case BLOCKS_CLEARING:
            if (timer_elapsed32(blocks_phase_at) < BLOCKS_CLEAR_MS) break;
            blocks_collapse();
            blocks_spawn();
            break;
        case BLOCKS_OVER:
            if (timer_elapsed32(blocks_phase_at) > GAME_OVER_BLINK_MS + GAME_SCORE_MS) blocks_phase = BLOCKS_OFF;
            break;
    }
}

// --- Key handling ------------------------------------------------------------

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool    pressed = record->event.pressed;
    uint8_t row     = record->event.key.row;
    uint8_t col     = record->event.key.col;
    bool    in_grid = IS_KEYEVENT(record->event) && row < MATRIX_ROWS && col < MATRIX_COLS;

    if (in_grid && !pressed) bounce_open_at[row][col] = 0;

    if (pressed && signal_kind == SIGNAL_ATTENTION) signal_kind = 0;

    // While a game runs, key presses control it instead of reaching the host.
    // Fn and volume still work. Releases are not swallowed: a key that was held
    // when the game started would otherwise stay held on the host.
    bool gaming = snake_phase || blocks_phase;
    if (gaming && keycode != SNAKE_GAME && keycode != BLOCKS_GAME && !IS_QK_MOMENTARY(keycode) && keycode != KC_VOLU && keycode != KC_VOLD && keycode != KC_MUTE) {
        if (blocks_phase) {
            blocks_key(keycode, pressed);
        } else if (pressed) {
            snake_key(keycode);
        }
        if (pressed) return false;
    }

    switch (keycode) {
        case SNAKE_GAME:
        case BLOCKS_GAME:
            if (pressed) {
                if (gaming) {
                    snake_phase  = SNAKE_OFF;
                    blocks_phase = BLOCKS_OFF;
                } else if (keycode == SNAKE_GAME) {
                    snake_start();
                } else {
                    blocks_start();
                }
            }
            return false;
        case BOOT_HOLD:
            boot_hold_at = pressed ? stamp() : 0;
            return false;
        case BOUNCE_RPT:
            if (pressed) send_bounce_report();
            return false;
        case KC_CAPS:
            if (pressed) {
                recording    = caps_up_at && !key_since_caps && timer_elapsed32(caps_up_at) < HYPER_DOUBLE_TAP_MS;
                caps_down_at = stamp();
            } else {
                if (recording) sent_flash_at = stamp();
                recording      = false;
                caps_down_at   = 0;
                caps_up_at     = stamp();
                key_since_caps = false;
            }
            return true; // Karabiner turns Caps into Hyper; it must still reach the host.
        default:
            break;
    }

    if (!pressed) return true;
    key_since_caps = true;

    // Hyper+I/U/P switch the input source in Karabiner; show that language's flag.
    if (caps_down_at && !recording && (keycode == KC_I || keycode == KC_U || keycode == KC_P)) {
        flag_shown = keycode == KC_I ? FLAG_ENGLISH : keycode == KC_U ? FLAG_UKRAINIAN : FLAG_POLISH;
        flag_at    = stamp();
    }

    // Knob and the F11/F12 media keys alike; macOS unmutes on any volume change.
    if (keycode == KC_VOLU || keycode == KC_VOLD) {
        volume_muted = false;
        volume_changed(keycode == KC_VOLU ? 1 : -1);
    } else if (keycode == KC_MUTE) {
        volume_muted = !volume_muted;
        volume_changed(0);
    }

    if (keycode == KC_BSPC) {
        if (rage_count < UINT8_MAX) rage_count++;
    } else if (!IS_MODIFIER_KEYCODE(keycode) && rage_count) {
        rage_fade_from = rage_count;
        rage_fade_at   = stamp();
        rage_count     = 0;
    }

    if (wtf_watch(keycode)) {
        signal_kind = SIGNAL_DOOM;
        signal_at   = stamp();
    } else if (keycode == KC_ENT && in_grid) {
        uint8_t led = g_led_config.matrix_co[row][col];
        if (led != NO_LED) {
            shock_at      = stamp();
            shock_x       = g_led_config.point[led].x;
            shock_y       = g_led_config.point[led].y;
            shock_rainbow = get_mods() & MOD_MASK_GUI;
        }
    }
    return true;
}

// Keychron's housekeeping_task_kb never calls housekeeping_task_user; this is
// the hook it does call.
void keychron_task_kb(void) {
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            if (bounce_open_at[r][c] && timer_elapsed32(bounce_open_at[r][c]) > BOUNCE_CONFIRM_MS) {
                bounce_open_at[r][c] = 0;
                record_bounce(r, c);
            }
            if (bounce_flash_at[r][c] && timer_elapsed32(bounce_flash_at[r][c]) > BOUNCE_FLASH_MS) {
                bounce_flash_at[r][c] = 0;
            }
        }
    }
    if (boot_hold_at && timer_elapsed32(boot_hold_at) > BOOT_HOLD_MS) reset_keyboard();
    snake_task();
    blocks_task();
    if (signal_kind == SIGNAL_ATTENTION && timer_elapsed32(signal_at) > SIGNAL_ATTENTION_MAX_MS) signal_kind = 0;
}

// --- Host signals --------------------------------------------------------------

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    if (data[1] == id_custom_channel && data[2] == SIGNAL_MAGIC && length > 3) {
        uint8_t kind = data[3];
        if ((kind >= SIGNAL_DONE && kind <= SIGNAL_ERROR) || kind == SIGNAL_DOOM) {
            signal_kind = kind;
            signal_at   = stamp();
            return;
        }
        if (kind == SIGNAL_CLEAR) {
            signal_kind = 0;
            flag_shown  = FLAG_NONE;
            return;
        }
        if (kind == SIGNAL_WHITE_BALANCE && length > 5) {
            white_balance_g = data[4];
            white_balance_b = data[5];
            return;
        }
        if (kind == SIGNAL_FLAG_TEST) {
            flag_shown = FLAG_TEST;
            flag_at    = stamp();
            return;
        }
    }
    data[0] = id_unhandled;
}

// --- Lighting overlays -------------------------------------------------------

static uint8_t brightness;

static void paint(uint8_t led, uint8_t h, uint8_t s, uint8_t v) {
    rgb_t rgb = white_balance(hsv_to_rgb((hsv_t){h, s, scale8(v, brightness)}));
    rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
}

static uint8_t fade_out(uint32_t elapsed, uint32_t duration) {
    return elapsed >= duration ? 0 : 255 - elapsed * 255 / duration;
}

static uint16_t distance(uint8_t led, uint8_t x, uint8_t y) {
    int16_t dx = g_led_config.point[led].x - x;
    int16_t dy = g_led_config.point[led].y - y;
    return sqrt16(dx * dx + dy * dy);
}

// Runs the body with `led`, `row` and `col` bound, for every key whose LED is
// in [led_min, led_max). Variadic because bodies contain commas.
#define FOR_EACH_KEY_LED(...)                                               \
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {                       \
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {                   \
            uint8_t led = g_led_config.matrix_co[row][col];                 \
            if (led == NO_LED || led < led_min || led >= led_max) continue; \
            __VA_ARGS__                                                     \
        }                                                                   \
    }

// Hell mode: a white/red strobe, then flames rising from the bottom row under a
// heartbeat, with W, T and F blinking; the fire dies down over the last second.
static void draw_doom(uint32_t t, uint8_t led_min, uint8_t led_max) {
    if (t > DOOM_MS) {
        signal_kind = 0;
        return;
    }
    if (t < DOOM_STROBE_MS) {
        uint8_t step = (t / 80) % 3;
        for (uint8_t i = led_min; i < led_max; i++) paint(i, 0, step == 0 ? 0 : 255, step == 2 ? 0 : 255);
        return;
    }
    uint8_t beat_t = (t % 900) / 4; // two thumps per 900 ms
    uint8_t beat   = (beat_t < 30 || (beat_t > 55 && beat_t < 85)) ? 255 : 150;
    uint8_t dying  = t > DOOM_FADE_FROM_MS ? fade_out(t - DOOM_FADE_FROM_MS, DOOM_MS - DOOM_FADE_FROM_MS) : 255;
    for (uint8_t i = led_min; i < led_max; i++) {
        uint8_t x      = g_led_config.point[i].x;
        uint8_t depth  = 64 - g_led_config.point[i].y; // 0 on the bottom row
        uint8_t height = 22 + scale8(sin8(x * 4 + t / 5), 22) + scale8(sin8(x * 11 - t / 3), 16);
        height         = scale8(height, dying);
        if (depth >= height) {
            paint(i, 0, 255, scale8(18, beat)); // embers above the flames
            continue;
        }
        uint8_t heat = 255 - depth * 255 / height;
        uint8_t sat  = heat > 225 ? 255 - (heat - 225) * 4 : 255; // white-hot at the base
        uint8_t val  = scale8(scale8(heat, 200 + random8() % 56), beat);
        paint(i, heat / 8, sat, val);
    }
    if (dying < 255) return;
    bool    white = (t / 125) % 2;
    uint8_t layer = get_highest_layer(default_layer_state);
    FOR_EACH_KEY_LED({
        uint16_t kc = keymap_key_to_keycode(layer, (keypos_t){.row = row, .col = col});
        if (kc == KC_W || kc == KC_T || kc == KC_F) paint(led, 0, white ? 0 : 255, 255);
    })
}

static void draw_host_signal(uint8_t led_min, uint8_t led_max) {
    if (!signal_kind) return;
    uint32_t t = timer_elapsed32(signal_at);
    switch (signal_kind) {
        case SIGNAL_DONE: {
            // Two green sweeps left to right.
            if (t > SIGNAL_DONE_MS) {
                signal_kind = 0;
                return;
            }
            uint16_t front = (t % (SIGNAL_DONE_MS / 2)) * 280 / (SIGNAL_DONE_MS / 2);
            for (uint8_t i = led_min; i < led_max; i++) {
                int16_t d = front - g_led_config.point[i].x;
                if (d >= 0 && d < 56) paint(i, 85, 255, 255 - d * 4);
            }
        } break;
        case SIGNAL_ATTENTION: {
            uint8_t v = 60 + scale8(sin8(t >> 3), 195);
            for (uint8_t i = led_min; i < led_max; i++) paint(i, 28, 255, v);
        } break;
        case SIGNAL_ERROR: {
            if (t > SIGNAL_ERROR_MS) {
                signal_kind = 0;
                return;
            }
            bool on = (t / 150) % 2 == 0 && t < 600;
            for (uint8_t i = led_min; i < led_max; i++) paint(i, 0, 255, on ? 255 : 0);
        } break;
        case SIGNAL_DOOM:
            draw_doom(t, led_min, led_max);
            break;
    }
}

static void draw_rage(uint8_t led_min, uint8_t led_max) {
    uint8_t level = rage_count;
    uint8_t fade  = 255;
    if (!level && rage_fade_at) {
        fade = fade_out(timer_elapsed32(rage_fade_at), RAGE_FADE_MS);
        if (!fade) {
            rage_fade_at = 0;
            return;
        }
        level = rage_fade_from;
    }
    if (level < 3) return;
    if (level > RAGE_FULL) level = RAGE_FULL;
    // Fills from Backspace (right) towards the left as the streak grows.
    uint8_t edge = 224 - (uint16_t)level * 224 / RAGE_FULL;
    for (uint8_t i = led_min; i < led_max; i++) {
        if (g_led_config.point[i].x < edge) continue;
        uint8_t v = level == RAGE_FULL ? 120 + (random8() % 136) : 220;
        paint(i, 0, 255, scale8(v, fade));
    }
}

static void draw_shockwave(uint8_t led_min, uint8_t led_max) {
    if (!shock_at) return;
    uint32_t t = timer_elapsed32(shock_at);
    if (t > SHOCK_MS) {
        shock_at = 0;
        return;
    }
    uint16_t radius = t * 260 / SHOCK_MS;
    uint8_t  fade   = fade_out(t, SHOCK_MS);
    for (uint8_t i = led_min; i < led_max; i++) {
        int16_t d = (int16_t)distance(i, shock_x, shock_y) - radius;
        if (d < -18 || d > 6) continue;
        uint8_t h = shock_rainbow ? (uint8_t)(g_led_config.point[i].x + t / 3) : 36;
        paint(i, h, shock_rainbow ? 255 : 200, fade);
    }
}

// Distance from the knob corner along a 45° diagonal, in hundredths of a key.
// LED x spans 224 units over ~15.5 keys and y spans 64 over 5 rows.
static uint16_t knob_distance(uint8_t led) {
    return (224 - g_led_config.point[led].x) * 100 / 145 + g_led_config.point[led].y * 100 / 128;
}

static void draw_volume(uint8_t led_min, uint8_t led_max) {
    if (!volume_changed_at) return;
    uint32_t t = timer_elapsed32(volume_changed_at);
    if (t > VOLUME_SHOW_MS + VOLUME_FADE_MS) {
        volume_changed_at = 0;
        return;
    }
    uint8_t fade = t < VOLUME_SHOW_MS ? 255 : fade_out(t - VOLUME_SHOW_MS, VOLUME_FADE_MS);

    static uint16_t far_end = 0; // distance of the bottom-left key: 100% volume
    if (!far_end) {
        for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) far_end = MAX(far_end, knob_distance(i));
    }
    uint16_t front = (uint32_t)volume_position() * far_end / (VOLUME_STEPS * 256);

    for (uint8_t i = led_min; i < led_max; i++) {
        uint16_t d = knob_distance(i);
        if (volume_muted) {
            paint(i, 0, 255, d < 130 ? fade : 0);
            continue;
        }
        // Lit keys run green at the knob to red at full volume; the key the
        // front is crossing is lit partially, so the wave slides between steps.
        uint8_t level = d <= front ? 255 : d < front + 80 ? 255 - (d - front) * 255 / 80 : 0;
        uint8_t hue   = 85 - (uint32_t)85 * d / far_end;
        paint(i, hue, 255, scale8(level, fade));
    }
}

static void draw_fn_legend(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer != MAC_FN && layer != WIN_FN) return;
    FOR_EACH_KEY_LED({
        uint16_t kc = keymap_key_to_keycode(layer, (keypos_t){.row = row, .col = col});
        if (kc == KC_TRNS || kc == KC_NO) {
            paint(led, 170, 255, 14);
        } else if (kc == BOOT_HOLD) {
            paint(led, 0, 255, 255);
        } else if (kc == BOUNCE_RPT) {
            paint(led, 213, 255, 255);
        } else if (kc >= KC_F1 && kc <= KC_F12) {
            paint(led, 28, 255, 255);
        } else if (IS_UNDERGLOW_KEYCODE(kc)) {
            paint(led, 0, 0, 255);
        } else {
            paint(led, 128, 255, 255);
        }
    })
}

// Karabiner's Hyper bindings: H/J/K/L arrows, F/B page down/up, Backspace
// forward delete, I/U/P English/Ukrainian/Polish, N Night Shift. Update together with
// ~/.config/karabiner/karabiner.json.
static void draw_hyper_sheet(uint8_t led_min, uint8_t led_max) {
    if (!caps_down_at || recording || timer_elapsed32(caps_down_at) < HYPER_SHEET_DELAY_MS) return;
    uint8_t  layer = get_highest_layer(default_layer_state);
    uint32_t now   = timer_read32();
    bool     flip  = (now / 500) % 2;
    FOR_EACH_KEY_LED({
        switch (keymap_key_to_keycode(layer, (keypos_t){.row = row, .col = col})) {
            case KC_H:
            case KC_J:
            case KC_K:
            case KC_L:
                paint(led, 0, 0, 150);
                break;
            case KC_F:
            case KC_B:
                paint(led, 128, 255, 255);
                break;
            case KC_BSPC:
                paint(led, 20, 255, 255);
                break;
            case KC_N: // night sky
                paint(led, 170, 255, 220);
                break;
            case KC_I: { // red, white, blue
                uint8_t step = (now / 400) % 3;
                paint(led, step == 2 ? 170 : 0, step == 1 ? 0 : 255, 255);
            } break;
            case KC_U: // blue / yellow
                paint(led, flip ? 43 : 160, 255, 255);
                break;
            case KC_P: // white / red
                paint(led, 0, flip ? 255 : 0, 255);
                break;
            case KC_CAPS:
                paint(led, 200, 255, 255);
                break;
            default:
                paint(led, 200, 255, 14);
                break;
        }
    })
}

// Union Jack on a -128..127 grid: red cross with white edges over red-on-white
// diagonals over blue.
static hsv_t union_jack(int16_t fx, int16_t fy) {
    const hsv_t red = {0, 255, 255}, white = {0, 0, 255}, blue = {162, 255, 190};
    int16_t     ax = abs(fx), ay = abs(fy);
    if (ax < 14 || ay < 26) return red;
    if (ax < 24 || ay < 44) return white;
    int16_t diag = MIN(abs(fx - fy), abs(fx + fy));
    if (diag < 14) return red;
    if (diag < 34) return white;
    return blue;
}

static void draw_flag(uint8_t led_min, uint8_t led_max) {
    if (!flag_shown) return;
    uint32_t t    = timer_elapsed32(flag_at);
    uint32_t show = flag_shown == FLAG_TEST ? FLAG_TEST_MS : FLAG_SHOW_MS;
    if (t > show + FLAG_FADE_MS) {
        flag_shown = FLAG_NONE;
        return;
    }
    uint8_t fade = t < show ? 255 : fade_out(t - show, FLAG_FADE_MS);

    const hsv_t white = {0, 0, 255}, red = {0, 255, 255}, blue = {150, 255, 255}, yellow = {40, 255, 255};
    for (uint8_t i = led_min; i < led_max; i++) {
        uint8_t x   = g_led_config.point[i].x;
        uint8_t y   = g_led_config.point[i].y;
        bool    top = y < 32; // rows 0-2 of 6
        uint8_t flag = flag_shown == FLAG_TEST ? (x < 112 ? FLAG_POLISH : FLAG_UKRAINIAN) : flag_shown;
        hsv_t   c;
        switch (flag) {
            case FLAG_POLISH:
                c = top ? white : red;
                break;
            case FLAG_UKRAINIAN:
                c = top ? blue : yellow;
                break;
            default:
                c = union_jack((int16_t)x * 255 / 224 - 128, (int16_t)y * 255 / 64 - 128);
                break;
        }
        // A ripple travels left to right, as if the flag were waving. The test
        // view holds still so colours can be judged.
        uint8_t wave = flag_shown == FLAG_TEST ? 255 : 175 + scale8(sin8(x * 2 - t / 3), 80);
        paint(i, c.h, c.s, scale8(scale8(c.v, wave), fade));
    }
}

static void draw_recording(uint8_t led_min, uint8_t led_max) {
    if (recording) {
        // "On air" light while Wispr Flow is listening: the board goes dark
        // except Caps and the right-hand column, which stay solid red.
        for (uint8_t i = led_min; i < led_max; i++) rgb_matrix_set_color(i, 0, 0, 0);
        uint8_t layer = get_highest_layer(default_layer_state);
        FOR_EACH_KEY_LED({
            uint16_t kc = keymap_key_to_keycode(layer, (keypos_t){.row = row, .col = col});
            if (kc == KC_CAPS || kc == KC_PGUP || kc == KC_PGDN || kc == KC_HOME) paint(led, 0, 255, 255);
        })
        return;
    }
    if (sent_flash_at) {
        uint8_t fade = fade_out(timer_elapsed32(sent_flash_at), SENT_FLASH_MS);
        if (!fade) {
            sent_flash_at = 0;
            return;
        }
        for (uint8_t i = led_min; i < led_max; i++) paint(i, 85, 255, fade);
    }
}

static void draw_bounces(uint8_t led_min, uint8_t led_max) {
    FOR_EACH_KEY_LED({
        if (!bounce_flash_at[row][col]) continue;
        uint32_t t = timer_elapsed32(bounce_flash_at[row][col]);
        // Strobe for the first half second so it is noticeable, then fade.
        uint8_t v = t < 500 ? ((t / 80) % 2 ? 80 : 255) : fade_out(t - 500, BOUNCE_FLASH_MS - 500);
        paint(led, 0, 255, v);
    })
}

static void draw_boot_progress(uint8_t led_min, uint8_t led_max) {
    if (!boot_hold_at) return;
    uint16_t fill = timer_elapsed32(boot_hold_at) * 224 / BOOT_HOLD_MS;
    for (uint8_t i = led_min; i < led_max; i++) {
        paint(i, 191, 255, g_led_config.point[i].x <= fill ? 255 : 10);
    }
}

static void paint_key(uint8_t row, uint8_t col, uint8_t led_min, uint8_t led_max, uint8_t h, uint8_t s, uint8_t v) {
    uint8_t led = g_led_config.matrix_co[row][col];
    if (led >= led_min && led < led_max) paint(led, h, s, v);
}

// A game's score on the number row: tens digit in blue, ones digit in white.
// When both digits are the same key, it alternates between the colours.
static void draw_game_number(uint8_t n, uint8_t led_min, uint8_t led_max, uint8_t level) {
    uint8_t tens = n / 10 % 10, ones = n % 10;
    bool    shared = tens && tens == ones;
    bool    blue   = (timer_read32() / 400) % 2;
    if (tens && (!shared || blue)) paint_key(1, tens, led_min, led_max, 170, 255, level);
    if (!shared || !blue) paint_key(1, ones ? ones : 10, led_min, led_max, 0, 0, level);
}

static void snake_paint(uint8_t cell, uint8_t led_min, uint8_t led_max, uint8_t h, uint8_t s, uint8_t v) {
    paint_key(SNAKE_ROW(cell), SNAKE_COL(cell), led_min, led_max, h, s, v);
}

// Returns true while the game owns the board.
static bool draw_snake(uint8_t led_min, uint8_t led_max) {
    if (!snake_phase) return false;
    for (uint8_t i = led_min; i < led_max; i++) rgb_matrix_set_color(i, 0, 0, 0);
    uint32_t t = timer_elapsed32(snake_phase_at);

    if (snake_phase == SNAKE_OVER && t > GAME_OVER_BLINK_MS) {
        draw_game_number(snake_len, led_min, led_max, 255);
        return true;
    }
    // Under the snake and the food, which may cross the number row.
    draw_game_number(snake_len, led_min, led_max, SNAKE_LIVE_LENGTH_LEVEL);

    uint8_t hue = 85, level = 255;
    if (snake_phase == SNAKE_OVER) {
        hue   = 0;
        level = (t / 150) % 2 ? 30 : 255;
    } else if (snake_phase == SNAKE_PAUSED) {
        level = 70 + scale8(sin8(timer_read32() >> 2), 110);
    }
    for (uint8_t i = snake_len; i-- > 1;) {
        // Fades towards the tail, which shows the direction of travel.
        uint8_t v = 230 - (uint16_t)i * 150 / snake_len;
        snake_paint(snake_body[i], led_min, led_max, hue, 255, scale8(v, level));
    }
    snake_paint(snake_body[0], led_min, led_max, hue, 110, level);
    if (snake_phase != SNAKE_OVER) {
        snake_paint(snake_food, led_min, led_max, 0, 255, 170 + scale8(sin8(timer_read32() >> 1), 85));
    }
    return true;
}

#define BLOCKS_EMPTY_LEVEL 16 // faint, so the outline of the well shows

// Returns true while the game owns the board.
static bool draw_blocks(uint8_t led_min, uint8_t led_max) {
    if (!blocks_phase) return false;
    for (uint8_t i = led_min; i < led_max; i++) rgb_matrix_set_color(i, 0, 0, 0);
    uint32_t t = timer_elapsed32(blocks_phase_at);

    if (blocks_phase == BLOCKS_OVER && t > GAME_OVER_BLINK_MS) {
        draw_game_number(blocks_cleared, led_min, led_max, 255);
        return true;
    }

    uint8_t level = 255;
    if (blocks_phase == BLOCKS_OVER) {
        level = (t / 150) % 2 ? 30 : 255;
    } else if (blocks_phase == BLOCKS_PAUSED) {
        level = 70 + scale8(sin8(timer_read32() >> 2), 110);
    }
    for (uint8_t lane = 0; lane < BLOCKS_LANES; lane++) {
        for (uint8_t depth = 0; depth < BLOCKS_DEPTH; depth++) {
            uint8_t led = blocks_led[lane][depth];
            if (led < led_min || led >= led_max) continue;
            uint8_t cell = blocks_well[lane][depth];
            if (!cell) {
                paint(led, 0, 0, BLOCKS_EMPTY_LEVEL);
            } else if (blocks_phase == BLOCKS_CLEARING && blocks_column_full(depth)) {
                paint(led, 0, 0, 255);
            } else if (blocks_phase == BLOCKS_OVER) {
                paint(led, 0, 255, level);
            } else {
                paint(led, blocks_shapes[cell - 1].hue, 255, scale8(150, level));
            }
        }
    }
    if (blocks_phase == BLOCKS_FALLING || blocks_phase == BLOCKS_PAUSED) {
        for (uint8_t i = 0; i < blocks_shapes[blocks_kind].cells; i++) {
            int8_t depth = blocks_depth + blocks_piece[i].depth;
            if (depth >= BLOCKS_DEPTH) continue; // still above the rim
            uint8_t led = blocks_led[blocks_lane + blocks_piece[i].lane][depth];
            if (led >= led_min && led < led_max) paint(led, blocks_shapes[blocks_kind].hue, 255, level);
        }
    }
    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Overlays follow the brightness knob, but never vanish completely.
    brightness = rgb_matrix_get_val() < 60 ? 60 : rgb_matrix_get_val();

    if (draw_snake(led_min, led_max) || draw_blocks(led_min, led_max)) return false;

    // Later layers draw over earlier ones.
    draw_host_signal(led_min, led_max);
    draw_rage(led_min, led_max);
    draw_shockwave(led_min, led_max);
    draw_fn_legend(led_min, led_max);
    draw_volume(led_min, led_max);
    draw_hyper_sheet(led_min, led_max);
    draw_flag(led_min, led_max);
    draw_recording(led_min, led_max);
    draw_bounces(led_min, led_max);
    draw_boot_progress(led_min, led_max);
    return false;
}
