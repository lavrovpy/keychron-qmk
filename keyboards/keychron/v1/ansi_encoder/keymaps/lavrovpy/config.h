#pragma once

// Applied only when the EEPROM is reset (first boot after flashing). After that,
// Keychron Launcher's debounce setting is the source of truth.
// Eager press + deferred release: presses are not delayed, and a contact that
// re-closes within 30 ms of opening is treated as one press. 30 ms covers the
// 18 ms re-close gaps logged on this board.
#undef DEBOUNCE
#define DEBOUNCE 30
#define DEFAULT_DEBOUNCE_TYPE DEBOUNCE_ASYM_EAGER_DEFER_PER_KEY

#define RGB_MATRIX_DEFAULT_ON true
// Dim rather than off: switching the matrix off would also hide the overlays.
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_DIM
#define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
#define RGB_MATRIX_DEFAULT_SAT 255
#define RGB_MATRIX_DEFAULT_HUE 0
#define RGB_MATRIX_DEFAULT_SPD 80
// The Dim effect's white level is speed setting x this / 255, so 30 by default.
#define DIM_WHITE_MAX 96

// Green and blue gains out of 255; see white_balance.h.
#define WHITE_BALANCE_G 150
#define WHITE_BALANCE_B 140

// Must match Karabiner's hyper rule (basic.to_delayed_action_delay_milliseconds),
// or the "recording" animation will disagree with Wispr Flow's push-to-talk.
#define HYPER_DOUBLE_TAP_MS 400
