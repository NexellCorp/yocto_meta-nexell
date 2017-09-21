SRC_URI = " \
        http://files.nexell.co.kr:8080/yocto-build-reference-source/qt5-5.4/${QT_MODULE}.tar.gz \
"

SRCPV=""

INSANE_SKIP_${PN}-examples-dev += "dev-elf"
INSANE_SKIP_${PN} += "invalid-packageconfig dev-elf"
INSANE_SKIP_${PN}-dbg += "libdir"
