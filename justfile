# Required to override KB_INFO_JSON defined in common.just
set allow-duplicate-variables := true

KB := "k2he"
LAYER := "MAC_BASE"
KB_INFO_JSON := "keyboard.json"
import? '../common/justfile'
import? '../common/qmk_compile.just'
