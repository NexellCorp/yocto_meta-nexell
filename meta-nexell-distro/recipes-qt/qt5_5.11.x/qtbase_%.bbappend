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

QT_CONFIG_FLAGS += " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'ld-is-gold', '-use-gold-linker', '-no-use-gold-linker', d)} \
"

# do_configure_prepend() {
#     if [ ! -e ${B}/bin/qmake ]; then
#         #rm -rf ${B}/bin/qmake
#         mkdir -p ${B}/bin
#         cp -a ${OE_QMAKE_PATH_EXTERNAL_HOST_BINS}/qmake ${B}/bin/
#     fi
# }
