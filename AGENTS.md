# AGENTS.md

This file provides guidance to agentic coding tools working with code in this repository.

## Repository Overview

This is ErebusBat's fork of the QMK firmware repository with custom keymaps for multiple keyboards. It tracks three upstreams:
- **keychron** — `git@github.com:Keychron/qmk_firmware.git` (Keychron boards)
- **sys76** — `git@github.com:system76/qmk_firmware.git` (System76 Launch boards)
- **origin** — `git@github.com:ErebusBat/qmk_firmware.git` (this fork, originally forked from sys76)

## Git: Always use `command git`

```bash
command git <subcommand>
```
Shell aliases and wrappers around `git` (e.g. shell-integration hooks) can interfere with scripted
git use. Always invoke git as `command git` — it bypasses aliases and shell functions while still
resolving the real git binary through `PATH`.

## Branch Strategy

Each keyboard family lives on its own branch tracking its respective upstream:

| Branch | Upstream | Keyboards |
|---|---|---|
| `playground` | keychron/playground | Keychron C3 Pro |
| `k2_he_2025q3` | keychron/2025q3 | Keychron K2 HE (v1.2.x compatible) |
| `erebusbat-keyboard` | sys76/master | System76 Launch 1 (custom keymap) |
| `master` | sys76/master | System76 Launch 1 (default keymaps only) |

**Always check out the correct branch before working on a keyboard.**

## Git Worktrees

If a git worktree exists for the target keyboard branch, use that worktree (or attempt to) instead of switching branches in the current worktree. This avoids conflicts with a dirty working tree and keeps keyboard-specific work isolated.

## Cross-Branch Shared Files

The following files are shared across all branches and must be kept in sync:
- `AGENTS.md` — this file
- `activate.sh` — QMK build environment activation script

After updating a shared file on any branch, sync it to the other branches (`playground`, `k2_he_2025q3`, `master`, `erebusbat-keyboard`, `drop_ctrl`, `drop_shift_10key`):

```bash
# From the target branch, pull the file from the source branch:
command git checkout <source-branch> -- AGENTS.md activate.sh
command git commit -m "Sync shared files from <source-branch>" -- AGENTS.md activate.sh
```

After multi-branch syncs, push all updated branches together:

```bash
command git push origin k2_he_2025q3 playground erebusbat-keyboard master drop_ctrl drop_shift_10key
```

## Build Environment Setup (macOS)

```bash
curl -fsSL https://install.qmk.fm | sh
qmk setup -H $HOME/src/erebusbat/qmk_firmware
```

The install script installs the QMK CLI, ARM/AVR toolchains, and flashing utilities to `~/Library/Application Support/qmk`.

### Activating the build environment

The QMK toolchains are not on PATH by default. Before compiling, source the activation script in your current shell:

```bash
source activate.sh
```

This prepends the QMK toolchain paths for the current session only.

## Build Commands

**Prefer the `just` recipes** — they are the canonical interface. Each recipe sources
`activate.sh` automatically and wraps the underlying `qmk` CLI, so you do not need to activate the
toolchain or remember `-kb`/`-km` values yourself. Run `just --list` to see all recipes.

```bash
just build      # alias for `just compile`
just flash      # depends on compile, so it builds first, then flashes
just check      # open Karabiner-EventViewer to see what keycodes the OS receives (debugging)
just clean      # remove built files and start fresh
```

Each branch's local `justfile` presets `KB` (e.g. the `k2_he_2025q3` branch sets `KB := "k2he"`),
so on the branch for a given keyboard these recipes need no arguments. When `KB` is not preset
(e.g. invoking from the shared `common` dir), select the keyboard explicitly:

```bash
just KB=(sys76|c3pro|k2he|shift) <recipe>   # e.g. just KB=k2he flash
```

### Underlying commands (what the recipes wrap)

The recipes ultimately run the `qmk` CLI. `KEYMAP` is always `erebusbat`; `KB` maps to `-kb`:

| `KB` | branch | `-kb` value |
| --- | --- | --- |
| `c3pro` | `playground` | `keychron/c3_pro/ansi/red` |
| `k2he` | `k2_he_2025q3` | `keychron/k2_he/ansi` |
| `sys76` | `erebusbat-keyboard` | `system76/launch_1` |
| `shift` | — | `massdrop/shift` |

```bash
# e.g. what `just KB=k2he flash` runs, after sourcing activate.sh:
qmk compile -kb keychron/k2_he/ansi -km erebusbat
qmk flash   -kb keychron/k2_he/ansi -km erebusbat
```

### Flashing Workflow

**Required for flashing**: run `just flash` from a dedicated terminal pane split off from the
agent's pane — never from the agent's own pane. How that pane is created depends on the terminal
multiplexer hosting the session.

#### Herdr (preferred when the agent runs inside Herdr)

First verify the agent is running in a Herdr-managed pane:

```bash
test "${HERDR_ENV:-}" = 1
```

If the check fails, fall back to tmux below. Otherwise:

1. **Split a sibling pane** (preserves the working directory, keeps focus in the agent's pane):
   ```bash
   herdr pane split --current --direction right --cwd "$PWD" --no-focus
   ```
   Take the new pane id from `.result.pane.pane_id` in the JSON response.
2. **Run the flash in that pane**:
   ```bash
   herdr pane run <pane-id> "just flash"
   ```
   (`just flash` sources `activate.sh` and compiles first.)
3. **Monitor progress / read output**:
   ```bash
   # compile finished, dfu-util stage starting:
   herdr pane wait-output <pane-id> --regex "(?i)flashing for bootloader" --timeout 300000
   # flash outcome:
   herdr pane wait-output <pane-id> --regex "(?i)(downloaded successfully|error|cannot open|no dfu|failed)" --timeout 120000
   herdr pane read <pane-id> --source recent-unwrapped --lines 25
   ```
   Use `--match` for a literal substring or `--regex` for a Rust regex; omit `--timeout` to wait
   indefinitely.

Reuse the same flash pane for repeated build/flash cycles in a session instead of splitting a new
pane each time.

#### tmux (fallback when not inside Herdr)

1. **Find the current pane id via Bash**:
   ```bash
   tmux display-message -p '#{pane_id} #{session_name}:#{window_index}.#{pane_index}'
   ```
2. **Split that pane** (use the tmux tool) to create a new dedicated pane.
3. **First command in the new pane**:
   ```bash
   export CLAUDE_CODE_PANE=1
   ```
4. **Use that pane to run** `just flash` (it sources `activate.sh` and compiles first).

**Recommended for long-running tasks**: use the same dedicated-pane workflow (Herdr or tmux) for
lengthy compiles or debugging sessions.

**Benefits**:
- Persistent output history
- Can monitor flash progress
- Environment (activate.sh) persists across commands
- Easy to reference output later

### DFU Mode

To flash, the keyboard must be in DFU mode: hold **Esc** while plugging in the USB cable.

DFU mode is only needed at the **flash step** — compilation gives ~1–2 minutes of lead time, so
tell the user to Esc+replug as soon as `just flash` starts (or check first with `dfu-util -l`,
available after `source activate.sh`). If dfu-util runs with no DFU device attached it fails
fast; put the keyboard in DFU mode and re-run `just flash` (the cached compile makes the retry
quick).

### Per-keyboard documentation

Keyboard-specific docs (recovery procedures, hardware reset, EEPROM warnings) live in per-keyboard AGENTS.md files on their respective branches:
- **K2 HE**: `keyboards/keychron/k2_he/AGENTS.md` (branch: `k2_he_2025q3`)

## ErebusBat Keymap Architecture

All custom keymaps live under `keyboards/<vendor>/<board>/.../keymaps/erebusbat/` and share common conventions.

### Standard files per keymap
- `keymap.c` — layer definitions and `process_record_user`
- `erebusbat.h` — shared custom keycode defines (currently duplicated per keymap)
- `config.h` — mod-tap timing overrides

### Layer convention (Keychron boards)
All Keychron keymaps use 4 layers:
- `MAC_BASE` (0) — default Mac layout, always set as default layer
- `MAC_FN` (1) — Mac function layer, activated via `MO(MAC_FN)`
- `WIN_BASE` (2) — on C3 Pro this is repurposed as a **Windowing layer** (not Windows OS)
- `WIN_FN` (3) — Windows function layer

### Custom keycodes (`EB_` prefix)
Defined in `erebusbat.h` in each keymap directory:
- `EB_CAPS` — `MT(MOD_LCTL, KC_ESC)`: tap for Escape, hold for Left Control
- `EB_ESC_HYPR` — `MT(MOD_HYPR, KC_ESC)`: tap for Escape, hold for Hyper (Ctrl+Shift+Alt+Gui)
- `EB_SCRN` — custom keycode that sends `Cmd+Shift+4` (macOS screenshot region)

### Windowing macros (Raycast integration)
The `EB_` windowing defines use `LCAG()` (Left Ctrl+Alt+Gui) modifier combos bound to Raycast window management on macOS:
- `EB_FULLS` — fullscreen
- `EB_HLF_A/B` — left/right half
- `EB_3RD_A/B/C` — thirds
- `EB_23RD_A/B/C` — two-thirds
- `EB_QTR_A/B/C/D` — quarters (2x2 grid)
- `EB_6th_A-F` — sixths (2x3 grid)

### Mod-tap settings
Configured in each keymap's `config.h`:
- `TAPPING_TERM 200` — hold/tap threshold
- `PERMISSIVE_HOLD` — register hold immediately when another key is pressed
- `HOLD_ON_OTHER_KEY_PRESS` — complementary hold behavior

### MAC_FN layer patterns
The Fn layer maps vim-style arrows to HJKL and wraps bottom-row keys in `C()` (Control) for Ctrl+modifier combos on the left hand side.

## Code Style

- Use `// clang-format off` / `// clang-format on` around keymap matrix definitions to preserve alignment
- Keycode alignment in keymap matrices matters for readability — keep columns aligned
- Use `_______` for transparent keys (standard QMK convention)
- Custom keycodes use `EB_` prefix
- Copyright header: `Copyright 2025 @ ErebusBat`
