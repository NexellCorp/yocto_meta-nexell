require qt5-nexell.inc

SRC_URI += " \
    ${QT_GIT}/qt/qtwebengine-chromium.git;name=chromium;branch=${QT_MODULE_BRANCH_CHROMIUM};destsuffix=git/src/3rdparty \
    file://0001-functions.prf-Don-t-match-QMAKE_EXT_CPP-or-QMAKE_EXT.patch \
    file://0002-functions.prf-Make-sure-we-only-use-the-file-name-to.patch \
    file://0003-functions.prf-allow-build-for-linux-oe-g-platform.patch \
    file://0001-chromium-base.gypi-include-atomicops_internals_x86_g.patch \
"

SRC_URI[md5sum] = "939beb8218f42206d6a89883c2c8161d"
SRC_URI[sha256sum] = "f2e08e76218840bb7c9e489527c4aa0eada88c1205494e1914e0efc1b7498eb9"
