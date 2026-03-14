KEYBOARD := "system76/launch_1"
KEYMAP := "erebusbat"
LAYER := "L_BASE"
KEYMAP_DIR := "keyboards/" + KEYBOARD + "/keymaps/" + KEYMAP

alias build := compile

# Compile QMK Firmware
compile:
	. ./activate.sh && qmk compile -kb {{KEYBOARD}} -km {{KEYMAP}}

# Flash QMK Firmware
flash:
	. ./activate.sh && qmk flash -kb {{KEYBOARD}} -km {{KEYMAP}}

# Setup QMK Environment
setup-qmk:
	python3 -m pip install --user qmk
	qmk setup

[linux]
check:
	xev

alias e := edit
# Edit keymap
edit:
    $EDITOR ./keyboards/{{ KEYBOARD }}/keymaps/{{ KEYMAP }}/* ./justfile

vim-keymap km=KEYMAP kb=KEYBOARD:
	vim keyboards/{{ kb }}/keymaps/{{ km }}/*

render-keymap-ascii layer=LAYER min_key_width="" kb=KEYBOARD km=KEYMAP:
	ruby util/render_keymap_ascii.rb --keymap keyboards/{{kb}}/keymaps/{{km}}/keymap.c --info keyboards/{{kb}}/info.json --layer {{layer}} {{ if min_key_width != "" { "--min-key-width " + min_key_width } else { "" } }}

render-keymap-ascii-blank layer=LAYER min_key_width="" kb=KEYBOARD km=KEYMAP:
	ruby util/render_keymap_ascii.rb --keymap keyboards/{{kb}}/keymaps/{{km}}/keymap.c --info keyboards/{{kb}}/info.json --layer {{layer}} --blank {{ if min_key_width != "" { "--min-key-width " + min_key_width } else { "" } }}

format-keymap layer=LAYER min_key_width="" kb=KEYBOARD km=KEYMAP:
    ruby util/format_keymap.rb --keymap keyboards/{{kb}}/keymaps/{{km}}/keymap.c --info keyboards/{{kb}}/info.json --layer {{layer}} {{ if min_key_width != "" { "--min-key-width " + min_key_width } else { "" } }}
