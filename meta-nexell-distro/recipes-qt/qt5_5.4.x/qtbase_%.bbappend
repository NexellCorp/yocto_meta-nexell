require qt5-nexell.inc

# common for qtbase-native, qtbase-nativesdk and qtbase
SRC_URI += " \
    file://0001-Add-linux-oe-g-platform.patch \
    file://0002-qlibraryinfo-allow-to-set-qt.conf-from-the-outside-u.patch \
    file://0003-Add-external-hostbindir-option.patch \
    file://0004-qt_module-Fix-pkgconfig-and-libtool-replacements.patch \
    file://0005-Revert-eglfs-Print-the-chosen-config-in-debug-mode.patch \
    file://0006-qeglplatformintegration-Undefine-CursorShape-from-X..patch \
    file://0007-configure-bump-path-length-from-256-to-512-character.patch \
    file://0008-eglfs-fix-egl-error-for-platforms-only-supporting-on.patch \
    file://0009-QOpenGLPaintDevice-sub-area-support.patch \
    file://0010-Make-Qt5GuiConfigExtras.cmake-find-gl-es-include-dir.patch \
"

# specific for qtbase
SRC_URI += " \
    file://0011-qmake-don-t-build-it-in-configure-but-allow-to-build.patch \
    file://0012-Set-paths-for-target-properly.patch \
"

SRC_URI[md5sum] = "3b5450e708d922787effc155e8ec44fd"
SRC_URI[sha256sum] = "1218e28e008e2f53198608b79ba7aff97cfec74341b273e6718bd67ede43d54a"

PACKAGECONFIG_GL = "${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'gl', '', d)}"
PACKAGECONFIG_FB = "${@bb.utils.contains('DISTRO_FEATURES', 'directfb', 'directfb', '', d)}"
PACKAGECONFIG_X11 = "${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'xcb xvideo xsync xshape xrender xrandr xfixes xinput2 xinput xinerama gtkstyle xkb', '', d)}"
PACKAGECONFIG_MULTIMEDIA = "${@bb.utils.contains('DISTRO_FEATURES', 'pulseaudio', 'pulseaudio', '', d)}"

PACKAGECONFIG = " \
    ${PACKAGECONFIG_RELEASE} \
    ${PACKAGECONFIG_DEFAULT} \
    ${PACKAGECONFIG_OPENSSL} \
    ${PACKAGECONFIG_FB} \
    ${PACKAGECONFIG_GL} \
    ${PACKAGECONFIG_X11} \
    ${PACKAGECONFIG_FONTS} \
    ${PACKAGECONFIG_SYSTEM} \
    ${PACKAGECONFIG_MULTIMEDIA} \
    ${PACKAGECONFIG_DISTRO} \
"

NX_WAYLAND_EGL_1 = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-fb', 'nexell-drm-mali-sdl-fb', '', d)}"
NX_WAYLAND_EGL_2 = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-wayland', 'nexell-drm-mali-sdl-wayland', '', d)}"
NX_WAYLAND_QT = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-qt', 'nexell-drm-mali-qt', '', d)}"

RDEPENDS_qtbase-plugins += " ${NX_WAYLAND_EGL_1} \
                           ${NX_WAYLAND_EGL_2} \
                           ${NX_WAYLAND_QT} \
                         "
RDEPENDS_qtbase-examples += " ${NX_WAYLAND_EGL_1} \
                           ${NX_WAYLAND_EGL_2} \
                           ${NX_WAYLAND_QT} \
                         "
RDEPENDS_qtbase += " ${NX_WAYLAND_EGL_1} \
                     ${NX_WAYLAND_EGL_2} \
                     ${NX_WAYLAND_QT} \
                   "
