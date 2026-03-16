# AGENT.md

## Build And Flash (Drop Shift v1)

Use the repository `justfile` targets for compile/flash/clean.

- Compile: `just compile`
- Flash: `just flash`
- Clean: `just clean`

Do **not** use `qmk compile` or `qmk flash` for this board.

### Why

The Drop Shift v1 uses Massdrop's ARM_ATSAM stack. The current toolchain can compile the firmware, but standard QMK CLI flows are not supported for this board's build/flash path.

The `justfile` handles the required environment and tooling:

- Sets PATH to include the QMK toolchain (`~/Library/Application Support/qmk/bin`)
- Builds with `make massdrop/shift:erebusbat`
- Flashes with `~/bin/mdloader --first --download .build/massdrop_shift_erebusbat.bin --restart`

When updating firmware for this repo, always use the `just` targets above.
