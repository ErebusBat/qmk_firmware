KB := 'shift'
LAYER := ''
import? '../common/justfile'

# The Massdrop Shift (10key) uses an older ARM_ATSAM architecture that
# was removed from QMK after `0.26.x`.  However the
QMK_TOOLCHAIN := "$HOME/Library/Application Support/qmk/bin"
PATH_WITH_QMK := QMK_TOOLCHAIN + ":$HOME/.local/bin:" + env_var("PATH")
MDLOADER_PATH := "$HOME/bin/mdloader"

# Compile Firmware for Massdrop Shift 10key
[group('Compile & Flash')]
compile:
    @PATH="{{ PATH_WITH_QMK }}" make massdrop/shift:erebusbat

# Flash Firmware to Massdrop Shift 10key
[group('Compile & Flash')]
flash: compile
    {{ MDLOADER_PATH }} --first --download .build/massdrop_shift_erebusbat.bin --restart

# Clean Build
[group('Compile & Flash')]
clean:
    make clean
