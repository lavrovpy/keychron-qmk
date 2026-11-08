# Keychron V1 (ANSI, knob): lavrovpy keymap

Stock Keychron layout and features (VIA, Keychron Launcher), plus:

| What you do | What the board does |
|---|---|
| Type | Aurora that follows typing speed: cyan when idle, through blue and magenta to red at ~120 WPM; bottom rows flicker above 90 WPM. Each press leaves a white spark. Dims after 10 min idle. |
| A switch bounces | The debounce filter swallows it and that key strobes red. **Fn+B** types how many bounces each key has had since plug-in. |
| Hold Caps (Hyper) 0.3 s | Cheat sheet for the Karabiner Hyper bindings: H/J/K/L white, F/B cyan, Backspace orange, I/U/P in English/Ukrainian/Polish flag colours. |
| Hyper+I / U / P (switch language) | The language's flag waves across the board for 2.5 s: Union Jack, Ukraine (blue over yellow), Poland (white over red). |
| Double-tap and hold Caps (Wispr Flow) | "On air" light while you talk: the board goes dark except Caps, PgUp, PgDn and Home in solid red; green flash on release. |
| Turn the knob (or F11/F12, or press it to mute) | Volume meter for 1.5 s: lights fill from the knob corner diagonally towards the bottom-left, one step per click, green to red. Muted shows only the corner in red. The board counts clicks (macOS has 16 volume steps) rather than reading the Mac's volume; turning fully down or up re-aligns it. |
| Enter / Cmd+Enter | Gold / rainbow shockwave from the Enter key. |
| Backspace streak | Red fills in from the right; at 16 in a row the whole board flickers. |
| Hold Fn | Keys that do something on the Fn layer light up. |
| Hold Fn+Esc 2 s | Purple progress bar, then bootloader mode for flashing. |
| `kbsignal done` / `attention` / `error` / `clear` | Green sweep / amber breathing until a key is pressed / red flash / stop. |

## Debounce

Defaults to "eager press, deferred release", 30 ms: presses are sent immediately; a release is sent
only once the contact has stayed open for 30 ms. Change it in Keychron Launcher; only this algorithm
reports bounces to the red strobe. `kbsignal debounce` prints what the board is using.

## Build and flash

```sh
docker run --rm -v "$PWD":/qmk_firmware -w /qmk_firmware qmkfm/qmk_cli make keychron/v1/ansi_encoder:lavrovpy
keyboards/keychron/v1/ansi_encoder/keymaps/lavrovpy/tools/flash.sh keychron_v1_ansi_encoder_lavrovpy.bin
```

Then hold Fn+Esc for 2 s. `flash.sh --reset` also wipes stored settings. Every flash saves the
previous firmware to `backups/`; `keychron_v1_ansi_encoder_keychron.bin` is stock Keychron.

## Host signal protocol

32-byte raw HID output report to usage page `0xFF60`, usage `0x61`:
`[0x07, 0x00, 0x4C, signal]`, where signal is 1 done, 2 attention, 3 error, 4 clear.
