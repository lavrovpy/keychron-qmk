#!/usr/bin/env bash
# Flash the Keychron V1 over USB (STM32 DFU).
#   flash.sh [--reset] firmware.bin
# Start it, then put the board in bootloader mode: hold Fn+Esc for 2 s with this
# firmware, or unplug, hold Esc and plug back in. The script waits up to 10 min.
# --reset also wipes the stored settings (keymap, lighting, debounce) so the
# firmware's defaults apply; needed when coming from a different firmware.
# Each run first saves the whole flash, including settings, to backups/.
set -euo pipefail

reset=0
if [[ "${1:-}" == "--reset" ]]; then reset=1; shift; fi
bin=${1:?usage: flash.sh [--reset] firmware.bin}
dfu_id=0483:df11
backup_dir="$(cd "$(dirname "$0")" && pwd)/../backups"

echo "Waiting for the keyboard in bootloader mode..."
for _ in $(seq 1 600); do
    dfu-util -l 2>/dev/null | grep -q "$dfu_id" && break
    sleep 1
done
dfu-util -l 2>/dev/null | grep -q "$dfu_id" || { echo "Timed out: no bootloader found." >&2; exit 1; }

mkdir -p "$backup_dir"
backup="$backup_dir/v1-$(date +%Y%m%d-%H%M%S).bin"
# The STM32L432 has 256 KB of flash; the settings live at its end.
if dfu-util -a 0 -d "$dfu_id" -s 0x08000000:262144 -U "$backup"; then
    echo "Backup: $backup"
else
    echo "Backup failed; flashing anyway." >&2
fi

if (( reset )); then
    dfu-util -a 0 -d "$dfu_id" -s 0x08000000:mass-erase:force:leave -D "$bin"
else
    dfu-util -a 0 -d "$dfu_id" -s 0x08000000:leave -D "$bin"
fi
echo "Flashed $bin"
