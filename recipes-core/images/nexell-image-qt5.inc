DESCRIPTION = "Extend core-image by adding Qt5"
LICENSE = "GPLv2"

inherit core-image populate_sdk_qt5

SPLASH = "psplash-nexell"

IMAGE_FEATURES += "package-management ssh-server-dropbear tools-debug splash"

# Qt Packages
IMAGE_INSTALL += " \
     qtbase \
     qtbase-plugins \
     qtbase-fonts \
     qtdeclarative \
     qtdeclarative-plugins \
     qtdeclarative-qmlplugins \
     qtdeclarative-tools \
     qtmultimedia \
     qtmultimedia-plugins \
     qtmultimedia-qmlplugins \
     qtwebkit \
     qtwebkit-qmlplugins \
     qtsvg \
     qtsvg-plugins \
     qt3d \
     qt3d-qmlplugins \
     "

# Example application
IMAGE_INSTALL += " \
     touchpoints \
     qkeytest \
     qcolorcheck \
     tslib \
     tslib-conf \
     tslib-tests \
     tslib-calibrate \
     "