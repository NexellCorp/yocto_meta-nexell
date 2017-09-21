require qt5-nexell.inc

SRC_URI[md5sum] = "c13b98e7c760bedccbf70bea3c273818"
SRC_URI[sha256sum] = "48c52ad926c4a02b7facca2240abb3d1d00ed48aa42d9633ddf08fb950baee24"

SRC_URI += " \
    file://0002-assistant-help-fix-linking-of-dependent-libraries.patch \
    file://0003-add-noqtwebkit-configuration.patch \
"

EXTRA_QMAKEVARS_PRE = "${@bb.utils.contains('PACKAGECONFIG', 'qtwebkit', '', 'CONFIG+=noqtwebkit', d)}"
