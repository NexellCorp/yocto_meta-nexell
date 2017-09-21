require qt5-nexell.inc

SRC_URI[md5sum] = "3a46f9c9d759403a78c0d792c1b8a977"
SRC_URI[sha256sum] = "3bcaeb7240623e0e4b4936eb6e69029237b7313b0f71f83b39119d6b48a8d858"

SRC_URI += " \
    file://0001-qtwebkit-examples-enable-building-examples-by-defaul.patch \
"

RDEPENDS_${PN}-examples = "qtwebkit-qmlplugins"
RDEPENDS_${PN}-examples += "${@bb.utils.contains('PACKAGECONFIG_OPENSSL', 'openssl', 'ca-certificates', '', d)}"
