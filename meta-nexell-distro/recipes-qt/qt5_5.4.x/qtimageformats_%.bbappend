require qt5-nexell.inc

SRC_URI[md5sum] = "89cfceb1e7533933761e3106c4c51f2e"
SRC_URI[sha256sum] = "4395fbb6da2b1f1b505fd02e765c116ff35b1592b4ae8430b5a5311b55631987"

SRC_URI += "file://0001-qtimageformats.pro-Make-the-dependencies-determinist.patch"

EXTRA_QMAKEVARS_PRE = "${@bb.utils.contains('PACKAGECONFIG', 'libmng', 'CONFIG+=OE_LIBMNG_ENABLED', '', d)}"
EXTRA_QMAKEVARS_PRE += "${@bb.utils.contains('PACKAGECONFIG', 'jasper', 'CONFIG+=OE_JASPER_ENABLED', '', d)}"
EXTRA_QMAKEVARS_PRE += "${@bb.utils.contains('PACKAGECONFIG', 'libtiff', 'CONFIG+=OE_LIBTIFF_ENABLED', '', d)}"
EXTRA_QMAKEVARS_PRE += "${@bb.utils.contains('PACKAGECONFIG', 'libwebp', 'CONFIG+=OE_LIBWEBP_ENABLED', '', d)}"
