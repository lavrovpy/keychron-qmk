#pragma once

#include "color.h"
#include "lib/lib8tion/lib8tion.h"

// Keychron drives the red LEDs at about half the current of green and blue
// (SNLED27351_CURRENT_TUNE in ../../config.h: 0x50 vs 0xA6), so white looks
// blue, red with any blue looks violet and yellow looks green. Raising the red
// current instead would raise the board's total draw on a 500 mA USB port.
// Defaults live in config.h; `kbsignal wb G B` changes them until unplugged.
extern uint8_t white_balance_g, white_balance_b;

static inline rgb_t white_balance(rgb_t c) {
    return (rgb_t){.r = c.r, .g = scale8(c.g, white_balance_g), .b = scale8(c.b, white_balance_b)};
}
