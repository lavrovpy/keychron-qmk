// "Mood": a slow aurora whose colour and speed follow typing speed.
// Idle is cyan; it moves through blue and magenta to red at ~120 WPM,
// and the bottom rows flicker like flames above 90 WPM.
RGB_MATRIX_EFFECT(MOOD)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#    include "keyboard.h"
#    include "wpm.h"

#    define MOOD_IDLE_DIM_MS (10UL * 60 * 1000)
#    define MOOD_SPARKLE_MS 450
#    define MOOD_SPARKLE_RADIUS 26

static bool MOOD(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // Phase is accumulated rather than derived from the timer, so a change in
    // speed bends the animation instead of making it jump.
    static uint32_t phase      = 0;
    static uint32_t last_timer = 0;
    uint8_t         wpm        = get_current_wpm();
    if (g_rgb_timer != last_timer) {
        uint32_t speed = rgb_matrix_config.speed / 4 + 6 + (uint32_t)wpm * 2;
        phase += (g_rgb_timer - last_timer) * speed;
        last_timer = g_rgb_timer;
    }
    uint8_t p1 = phase >> 10;
    uint8_t p2 = phase >> 11;

    uint8_t base_hue = 140 + (wpm > 120 ? 120 : wpm) + rgb_matrix_config.hsv.h;
    bool    idle     = last_input_activity_elapsed() > MOOD_IDLE_DIM_MS;
    bool    on_fire  = wpm >= 90;

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        uint8_t x = g_led_config.point[i].x;
        uint8_t y = g_led_config.point[i].y;

        uint8_t w1  = sin8(x + p1);
        uint8_t w2  = sin8(y * 3 - p2 + (x >> 1));
        uint8_t hue = base_hue + ((int8_t)(w1 - 128) >> 3) + ((int8_t)(w2 - 128) >> 4);
        uint8_t sat = rgb_matrix_config.hsv.s;
        uint8_t val = scale8(rgb_matrix_config.hsv.v, 150 + scale8(w2, 105));

        if (on_fire && y > 40) val = scale8(val, 190 + (random8() % 66));
        if (idle) val = scale8(val, 20 + scale8(sin8(g_rgb_timer >> 5), 40));

#    ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
        // Recent key presses leave a white spark that fades out.
        for (uint8_t j = 0; j < g_last_hit_tracker.count; j++) {
            uint16_t tick = g_last_hit_tracker.tick[j];
            if (tick >= MOOD_SPARKLE_MS) continue;
            int16_t  dx   = x - g_last_hit_tracker.x[j];
            int16_t  dy   = y - g_last_hit_tracker.y[j];
            uint16_t dist = sqrt16(dx * dx + dy * dy);
            if (dist >= MOOD_SPARKLE_RADIUS) continue;
            uint8_t fade  = 255 - (uint32_t)tick * 255 / MOOD_SPARKLE_MS;
            uint8_t near  = 255 - dist * 255 / MOOD_SPARKLE_RADIUS;
            uint8_t spark = scale8(fade, near);
            sat           = qsub8(sat, spark);
            val           = qadd8(val, scale8(spark, rgb_matrix_config.hsv.v));
        }
#    endif

        rgb_t rgb = hsv_to_rgb((hsv_t){hue, sat, val});
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}
#endif
