KB := "shift"
LAYER := 'L_BASE'
import? '../common/justfile'

#
# The v1 massdrop boards use the ARM_ATSAM which IS supported
# as a toolchoin in current (Mar 2026) QMK toolchain; however
# the board definitions / flash *are not supported*.  So we
# running the QMK setup will install the compile toolchain,
# but you can't use a regular `qmk compile` to build it
# nor can you flash it qith `qmk flash`.
#
# Download / make the mdloader tool:
#   - https://github.com/Massdrop/qmk_firmware
#   - http://gitea.erebusbat.net/ErebusBat/mdloader
#

# Massdrop / ARM_ATSAM Custom Vars
QMK_TOOLCHAIN := "$HOME/Library/Application Support/qmk/bin"
PATH_WITH_QMK := QMK_TOOLCHAIN + ":$HOME/.local/bin:" + env_var("PATH")
MDLOADER_PATH := "$HOME/bin/mdloader"

# Compile QMK Firmware
[group('Compile & Flash')]
@compile:
    PATH="{{ PATH_WITH_QMK }}" make massdrop/shift:erebusbat

# Flash QMK Firmware using custom mdloader tool
[group('Compile & Flash')]
@flash: compile
    {{ MDLOADER_PATH }} --first --download .build/massdrop_shift_erebusbat.bin --restart

# Clean built files, start fresh
[group('Compile & Flash')]
@clean:
    PATH="{{ PATH_WITH_QMK }}" make clean
