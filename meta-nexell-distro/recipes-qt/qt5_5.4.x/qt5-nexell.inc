SRC_URI = " \
    ${QT_GIT}/qt/${QT_MODULE}.git;branch=${QT_MODULE_BRANCH} \
"

SRCPV=""

INSANE_SKIP_${PN}-examples-dev += "dev-elf"
INSANE_SKIP_${PN} += "invalid-packageconfig dev-elf"
INSANE_SKIP_${PN}-dbg += "libdir"
