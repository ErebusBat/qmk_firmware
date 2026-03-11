KEYBOARD := "system76/launch_1"
KEYMAP := "erebusbat"
KEYMAP_DIR := "keyboards/" + KEYBOARD + "/keymaps/" + KEYMAP

compile:
	. ./activate.sh && qmk compile -kb {{KEYBOARD}} -km {{KEYMAP}}

flash:
	. ./activate.sh && qmk flash -kb {{KEYBOARD}} -km {{KEYMAP}}

qmk:
	python3 -m pip install --user qmk
	qmk setup

check:
	xev

alias e := edit
edit:
	vimls

vim-keymap km=KEYMAP kb=KEYBOARD:
	vim keyboards/{{ kb }}/keymaps/{{ km }}/*
