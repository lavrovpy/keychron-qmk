/* Keychron V1 ANSI knob: stock Keychron layout plus lighting overlays.
 * Feature list and host protocol: README.md in this directory.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "keychron_debounce.h"
#include "via.h"
#include "lib/lib8tion/lib8tion.h"

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
        _______,            _______,  _______,  _______,  _______,  BOUNCE_RPT,_______, _______,  _______,  _______,  _______,            _______,  _______,
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
        _______,            _______,  _______,  _______,  _______,  BOUNCE_RPT,_______, _______,  _______,  _______,  _______,            _______,  _______,
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
#define SIGNAL_DONE_MS 1400
#define SIGNAL_ERROR_MS 900
#define SIGNAL_ATTENTION_MAX_MS 60000

// Host signals arrive as VIA "custom set value" on the custom channel:
// [0x07, 0x00, SIGNAL_MAGIC, signal]. The magic keeps them apart from any
// custom values a VIA/Launcher definition might add on channel 0.
#define SIGNAL_MAGIC 0x4C
enum host_signal {
    SIGNAL_DONE      = 1,
    SIGNAL_ATTENTION = 2,
    SIGNAL_ERROR     = 3,
    SIGNAL_CLEAR     = 4,
};

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

// --- Key handling ------------------------------------------------------------

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool    pressed = record->event.pressed;
    uint8_t row     = record->event.key.row;
    uint8_t col     = record->event.key.col;
    bool    in_grid = IS_KEYEVENT(record->event) && row < MATRIX_ROWS && col < MATRIX_COLS;

    if (in_grid && !pressed) bounce_open_at[row][col] = 0;

    if (pressed && signal_kind == SIGNAL_ATTENTION) signal_kind = 0;

    switch (keycode) {
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

    if (keycode == KC_BSPC) {
        if (rage_count < UINT8_MAX) rage_count++;
    } else if (!IS_MODIFIER_KEYCODE(keycode) && rage_count) {
        rage_fade_from = rage_count;
        rage_fade_at   = stamp();
        rage_count     = 0;
    }

    if (keycode == KC_ENT && in_grid) {
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
    if (signal_kind == SIGNAL_ATTENTION && timer_elapsed32(signal_at) > SIGNAL_ATTENTION_MAX_MS) signal_kind = 0;
}

// --- Host signals --------------------------------------------------------------

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    if (data[1] == id_custom_channel && data[2] == SIGNAL_MAGIC && length > 3) {
        uint8_t kind = data[3];
        if (kind >= SIGNAL_DONE && kind <= SIGNAL_ERROR) {
            signal_kind = kind;
            signal_at   = stamp();
            return;
        }
        if (kind == SIGNAL_CLEAR) {
            signal_kind = 0;
            return;
        }
    }
    data[0] = id_unhandled;
}

// --- Lighting overlays -------------------------------------------------------

static uint8_t brightness;

static void paint(uint8_t led, uint8_t h, uint8_t s, uint8_t v) {
    rgb_t rgb = hsv_to_rgb((hsv_t){h, s, scale8(v, brightness)});
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
// forward delete, I/U/P English/Ukrainian/Polish. Update together with
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
                paint(led, 0, 0, 255);
                break;
            case KC_F:
            case KC_B:
                paint(led, 128, 255, 255);
                break;
            case KC_BSPC:
                paint(led, 20, 255, 255);
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

static void draw_recording(uint8_t led_min, uint8_t led_max) {
    if (recording) {
        // Rings pulse out from the space bar while Wispr Flow is listening.
        uint32_t t = timer_elapsed32(caps_down_at);
        for (uint8_t i = led_min; i < led_max; i++) {
            uint16_t d    = distance(i, 94, 61);
            uint8_t  wave = sin8(d * 3 - (t >> 1));
            paint(i, 248, 230, 40 + scale8(wave, 215));
        }
        // The right-hand column stays solid red: an "on air" light.
        uint8_t layer = get_highest_layer(default_layer_state);
        FOR_EACH_KEY_LED({
            uint16_t kc = keymap_key_to_keycode(layer, (keypos_t){.row = row, .col = col});
            if (kc == KC_PGUP || kc == KC_PGDN || kc == KC_HOME) paint(led, 0, 255, 255);
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

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Overlays follow the brightness knob, but never vanish completely.
    brightness = rgb_matrix_get_val() < 60 ? 60 : rgb_matrix_get_val();

    // Later layers draw over earlier ones.
    draw_host_signal(led_min, led_max);
    draw_rage(led_min, led_max);
    draw_shockwave(led_min, led_max);
    draw_fn_legend(led_min, led_max);
    draw_hyper_sheet(led_min, led_max);
    draw_recording(led_min, led_max);
    draw_bounces(led_min, led_max);
    draw_boot_progress(led_min, led_max);
    return false;
}
