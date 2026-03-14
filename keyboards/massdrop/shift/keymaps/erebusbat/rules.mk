# Massdrop ARM_ATSAM codebase defines variables in headers (not using extern),
# which GCC 10+ treats as multiple-definition errors under -fno-common (new default).
# -fcommon restores the old behavior (treat as tentative definitions, merge at link time).
EXTRAFLAGS += -fcommon
