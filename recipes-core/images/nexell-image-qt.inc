DESCRIPTION = "Extend core-image by adding Qt5"
LICENSE = "GPLv2"

inherit core-image populate_sdk_qt5
require recipes-graphics/images/core-image-weston.bb

WAYLAND_IMAGE_INSTALL = "${@base_contains('DISTRO_FEATURES', 'wayland', \
    'qtwayland \
     qtwayland-plugins', '', d)}"

QT_IMAGE_INSTALL_common = " \
     qtbase \
     qtbase-plugins \
     qtbase-examples \
     qtdeclarative \
     qtdeclarative-plugins \
     qtdeclarative-qmlplugins \
     qtdeclarative-tools \
     qtmultimedia \
     qtmultimedia-plugins \
     qtmultimedia-qmlplugins \
     qtquick1 \
     qtquickcontrols \
     qtquickcontrols2 \
     qtgraphicaleffects \
     ${WAYLAND_IMAGE_INSTALL} \
     glmark2 \
     "

QT_IMAGE_INSTALL_qtwebkit = " \
     qtwebkit \
     qtwebkit-qmlplugins \
     qtsvg \
     qtsvg-plugins \
     "

QT_IMAGE_INSTALL_qtwebengine = " \
     qtwebengine \
     "
     
QT_IMAGE_INSTALL_qt3d = "qt3d qt3d-qmlplugins qt3d-examples cinematicexperience"

EXAMPLE_IMAGE_INSTALL = " \
     touchpoints \
     qkeytest \
     qcolorcheck \
     "
TOUCH_IMAGE_INSTALL = " \
     tslib \
     tslib-conf \
     tslib-tests \
     tslib-calibrate \
     tslib-nexell \
     "

QT_ALSA_IMAGE_INSTALL = " \
     alsa-utils \
     alsa-lib \
     "
QT_ALSA_EXTRA_IMAGE_INSTALL = " \
     alsa-utils-alsamixer \
     alsa-utils-midi \
     alsa-utils-aplay \
     alsa-utils-amixer \
     alsa-utils-aconnect \
     alsa-utils-iecset \
     alsa-utils-speakertest \
     alsa-utils-aseqnet \
     alsa-utils-aseqdump \
     alsa-utils-alsactl \
     "

IMAGE_INSTALL += " \
     ${QT_IMAGE_INSTALL_common} \
     ${QT_IMAGE_INSTALL_qtwebkit} \
     ${EXAMPLE_IMAGE_INSTALL} \
     ${TOUCH_IMAGE_INSTALL} \
     ${QT_ALSA_IMAGE_INSTALL} \
     ${QT_ALSA_EXTRA_IMAGE_INSTALL} \
     podonamu \
     "

