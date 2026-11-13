TAP_DANCE_ENABLE = yes
OPT_DEFS += -DLED_MATRIX_CUSTOM_USER
KEY_OVERRIDE_ENABLE = yes

# Shared chord defines: ../common/eb_keys.h (ErebusBat/qmk-common)
EXTRAINCDIRS += $(TOP_DIR)/../common
