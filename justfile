QMK_TOOLCHAIN := "$HOME/Library/Application Support/qmk/bin"
PATH_WITH_QMK := QMK_TOOLCHAIN + ":$HOME/.local/bin:" + env_var("PATH")

compile:
    PATH="{{PATH_WITH_QMK}}" make massdrop/shift:erebusbat

flash:
    ~/bin/mdloader_mac --first --download .build/massdrop_shift_erebusbat.bin --restart
