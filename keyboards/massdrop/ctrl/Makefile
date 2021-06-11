KB ?= ctrl
KEYMAP ?= erebusbat
# MD_LOADER_DIR ?= /home/aburns/Dropbox/Library/D/drop_keyboard
QMK_BUILD ?= $(HOME)/src/vendor/qmk_firmware/.build
FW ?= $(QMK_BUILD)/massdrop_$(KB)_$(KEYMAP).hex

vim:
	vim keymaps/$(KEYMAP)/keymap.c Makefile

compile:
	qmk compile -kb massdrop/$(KB) -km $(KEYMAP)

flash: compile
	@printf "\n\n================================================================================\n"
	@printf "PUT KB INTO DFU MODE (Fn+b for 5s)\n"
	@printf "================================================================================\n"
	./mdloader_linux --first --restart --download $(FW)

qmk:
	python3 -m pip install --user qmk
	qmk setup

newkm:
	qmk new-keymap -kb massdrop/$(KB) -km $(KEYMAP)

check:
	xev | rg -A 5 '^KeyPress event'
