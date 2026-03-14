QMK_TOOLCHAIN := "$HOME/Library/Application Support/qmk/bin"
PATH_WITH_QMK := QMK_TOOLCHAIN + ":$HOME/.local/bin:" + env_var("PATH")
MDLOADER_PATH := "$HOME/bin/mdloader"

compile:
    PATH="{{ PATH_WITH_QMK }}" make massdrop/shift:erebusbat

flash:
    {{ MDLOADER_PATH }} --first --download .build/massdrop_shift_erebusbat.bin --restart
