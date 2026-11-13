# Keychron V1 (ANSI, knob): lavrovpy keymap

Stock Keychron layout and features (VIA, Keychron Launcher), plus:

| What you do | What the board does |
|---|---|
| Nothing | A very dim white glow by default (the Dim effect), so the effects below stand out. Fn+T / Fn+G make it brighter / dimmer in small steps, down to fully dark; the board remembers the level. Fn+Q / Fn+A step through effects, including Mood. |
| Type (Mood effect) | Aurora that follows typing speed: cyan when idle, through blue and magenta to red at ~120 WPM; bottom rows flicker above 90 WPM. Each press leaves a white spark. Dims after 10 min idle. |
| A switch bounces | The debounce filter swallows it and that key strobes red. **Fn+B** types how many bounces each key has had since plug-in. |
| Hold Caps (Hyper) 0.3 s | Cheat sheet for the Karabiner Hyper bindings: H/J/K/L white (arrows), F/B cyan (page down/up), Backspace orange (forward delete), N blue (Night Shift), I/U/P in English/Ukrainian/Polish flag colours. |
| Hyper+I / U / P (switch language) | The language's flag waves across the board for 2.5 s: Union Jack, Ukraine (blue over yellow), Poland (white over red). |
| Double-tap and hold Caps (Wispr Flow) | "On air" light while you talk: the board goes dark except Caps, PgUp, PgDn and Home in solid red; green flash on release. |
| Turn the knob (or F11/F12, or press it to mute) | Volume meter for 1.5 s: lights fill from the knob corner diagonally towards the bottom-left, one step per click, green to red. Muted shows only the corner in red. The board counts clicks (macOS has 16 volume steps) rather than reading the Mac's volume; turning fully down or up re-aligns it. |
| Enter / Cmd+Enter | Gold / rainbow shockwave from the Enter key. |
| Backspace streak | Red fills in from the right; at 16 in a row the whole board flickers. |
| Hold Fn | Keys that do something on the Fn layer light up. |
| Fn+N | Snake, played on the keys: green snake, red food, each key is one cell and all edges wrap, so only biting yourself ends the game. Steer with H/J/K/L, W/A/S/D or the arrows; Space pauses; Esc or Fn+N quits. Keys don't reach the computer while it runs (the knob still changes volume). The snake's length shows on the number row, tens digit in blue and ones digit in white (blue 1 and white 2 = 12; at 11, 22, … the one key alternates between the two colours): dimmer during play, then bright for 6 s after the red blink. Space plays again. |
| Fn+M | Blocks, a falling-blocks game in a well that lies on its side: five key rows wide (F-row to Shift row) and twelve keys deep, floor at the left edge. Pieces of one to three keys enter on the right and fall left; a full column of five (such as F1, 1, Q, A, Z) flashes white and clears. Turn the keyboard a quarter turn anticlockwise and the pieces fall towards you; the arrow keys turn with it. Up/Down (or K/J, W/S) move the piece across the well, Right (L, D) rotates it, holding Left (H, A) drops it faster, Space drops it at once, P pauses, Esc or Fn+M quits. It speeds up with every column cleared. After the red blink, the number row shows how many columns you cleared, in the same blue-tens / white-ones digits as Snake; Space plays again. |
| Hold Fn+Esc 2 s | Purple progress bar, then bootloader mode for flashing. |
| `kbsignal done` / `attention` / `error` / `clear` | Green sweep / amber breathing until a key is pressed / red flash / stop. |
| Type "wtf?" then Enter, anywhere | Hell mode for 7 s: white/red strobe, then flames rise from the bottom row under a heartbeat while W, T and F blink. The board matches physical keys, so "цеа?" on the Ukrainian layout counts too; Shift+Enter and Hyper/Cmd/Ctrl/Alt combos don't. `kbsignal doom` replays it. |

## Debounce

Defaults to "eager press, deferred release", 30 ms: presses are sent immediately; a release is sent
only once the contact has stayed open for 30 ms. Change it in Keychron Launcher; only this algorithm
reports bounces to the red strobe. `kbsignal debounce` prints what the board is using.

## Build and flash

```sh
docker run --rm -v "$PWD":/qmk_firmware -w /qmk_firmware qmkfm/qmk_cli make keychron/v1/ansi_encoder:lavrovpy
keyboards/keychron/v1/ansi_encoder/keymaps/lavrovpy/tools/flash.sh keychron_v1_ansi_encoder_lavrovpy.bin
```

Then hold Fn+Esc for 2 s. `flash.sh --reset` also wipes stored settings. The board keeps its key
layout in those settings, and on its own refreshes it from the firmware only when the build date
changes, so every flash ends by resetting the saved layout to `keymap.c` (`kbsignal keymap-reset`).
Remaps made in Keychron Launcher therefore last only until the next flash; put them in `keymap.c`. Every flash saves the
previous firmware to `backups/`; `keychron_v1_ansi_encoder_keychron.bin` is stock Keychron.

## Host signal protocol

32-byte raw HID output report to usage page `0xFF60`, usage `0x61`:
`[0x07, 0x00, 0x4C, signal, args...]`, where signal is 1 done, 2 attention, 3 error, 4 clear,
5 white balance (args: green gain, blue gain), 6 flag test, 7 doom.

## Colour balance

Keychron drives the red LEDs at about half the current of green and blue, so the firmware scales
green and blue down (`WHITE_BALANCE_G` / `_B` in `config.h`). To retune: `kbsignal flags` shows the
Polish and Ukrainian flags side by side for 20 s, `kbsignal wb GREEN BLUE` changes the gains live
until the board is unplugged; put the values that look right into `config.h`.
