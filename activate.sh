# Copyright 2025 @ ErebusBat
#
# QMK Build Environment Activation
# Usage: source activate.sh
#
# Adds the QMK toolchains (ARM, AVR, flashing tools) to your PATH
# for the current shell session only.
#
# Install the QMK toolchains first:
#   curl -fsSL https://install.qmk.fm | sh

_QMK_TOOLCHAIN_DIR="$HOME/Library/Application Support/qmk/bin"

if [ ! -d "$_QMK_TOOLCHAIN_DIR" ]; then
    echo "ERROR: QMK toolchain directory not found: $_QMK_TOOLCHAIN_DIR"
    echo ""
    echo "Install the QMK toolchains first:"
    echo "  curl -fsSL https://install.qmk.fm | sh"
    echo ""
    echo "See: https://docs.qmk.fm/newbs_getting_started"
    unset _QMK_TOOLCHAIN_DIR
    return 1
fi

export PATH="$_QMK_TOOLCHAIN_DIR:$HOME/.local/bin:$PATH"
unset _QMK_TOOLCHAIN_DIR

_QMK_GIT_BIN="git"
if [ -x "/opt/homebrew/bin/git" ]; then
    _QMK_GIT_BIN="/opt/homebrew/bin/git"
fi

if [ -d ".git" ] && [ ! -d "lib/chibios-contrib/os/common/startup/ARMCMx/compilers/GCC/mk" ]; then
    echo "QMK submodules missing; initializing..."
    "$_QMK_GIT_BIN" submodule update --init --recursive
fi

unset _QMK_GIT_BIN
echo "QMK build environment activated."
