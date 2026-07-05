# Digital Encoder Madness

A dual **KY-040 rotary-encoder** menu controller for Teensy/Arduino, written in 2019.
It reads two encoders and their push-buttons, drives a multi-level menu, and streams the
selected menu state to a host over serial - with a hand-rolled noise filter so the
notoriously jittery KY-040 actually behaves.

## What it does
- Reads **2x KY-040 encoders** (rotation) plus their integrated **push-buttons**.
- Menu model: **tabs** (encoder 1), **modes** (encoder 2), and a **submenu** driven by
  *chorded* button presses (hold one, tap the other).
- **Transmits only on change** - sends a menu label over serial when it actually changes,
  not every loop, to keep the link quiet.
- **Standby / wake** power control on a dedicated pin.
- Tiny serial command channel: `sleep`, `wake`, `identify` (replies `IDENTITY`).

## The part worth reading: KY-040 debounce
KY-040s are electrically noisy and throw spurious counts, which makes naive
`encoder.read()` menus jitter and skip. Instead of reaching for a library, this rolls its
own **time-windowed hysteresis filter** (`encArray` + `indexLimit`/`bufferLimit` +
`timeBuff`): it requires several *consistent* detents inside a time window before it
registers a menu move, resets the accumulator on a direction change, and times out stale
input.

## Hardware
- A **Teensy** (or any board supported by the PJRC Encoder library).
- 2x KY-040 rotary encoders (with push-buttons).
- 4 indicator LEDs.
- Optional power-switching on the power pin.

### Pin map (change at the top of the sketch)
| Function | Pin |
|---|---|
| Encoder 1 - A / B / button | 5 / 6 / 7 |
| Encoder 2 - A / B / button | 2 / 3 / 4 |
| LEDs - white / yellow / green / blue | 23 / 22 / 21 / 20 |
| Power | 34 |

## Dependencies
- [PJRC Encoder](https://www.pjrc.com/teensy/td_libs_Encoder.html) - `Encoder.h`
- [Bounce2](https://github.com/thomasfredericks/Bounce2) - `Bounce2.h`

## Serial protocol
- **Baud:** 38400
- **Sends** (on change): the current tab / mode / submenu label, plus `STANDING BY` /
  `WAKING UP`.
- **Receives** (newline-terminated): `sleep`, `wake`, `identify`.

## Known bugs (left in on purpose)
Published as-is, so the original bugs stay. If you want to *use* this, fix these first:
- **Off-by-one in LED init:** `for (i = 0; i <= LEDs; i++)` over a 4-element array writes
  one element out of bounds. Should be `<`.
- **Copy-paste drift in `menuTwo`:** sets `enc1move` where it means `enc2move`, and uses a
  couple of `timeBuff` indices that should point at 3/4.
- **Decorative parameters:** several functions take arguments they ignore and operate on
  globals instead.

## License
[MIT](LICENSE) - do whatever you want with it.
