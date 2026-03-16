KB := "sys76"
LAYER := "L_BASE"
import? '../common/justfile'
import? '../common/qmk_compile.just'

# Hide README case sensitivity noise on macOS
fix-readme-case:
    git update-index --assume-unchanged keyboards/system76/launch_1/README.md keyboards/system76/launch_2/README.md keyboards/system76/launch_heavy_1/README.md keyboards/system76/launch_lite_1/README.md

# Restore README tracking
unfix-readme-case:
    git update-index --no-assume-unchanged keyboards/system76/launch_1/README.md keyboards/system76/launch_2/README.md keyboards/system76/launch_heavy_1/README.md keyboards/system76/launch_lite_1/README.md
