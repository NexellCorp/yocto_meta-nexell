NX_WAYLAND_EGL_1 = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-fb', 'nexell-drm-mali-sdl-fb', '', d)}"
NX_WAYLAND_EGL_2 = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-wayland', 'nexell-drm-mali-sdl-wayland', '', d)}"
NX_WAYLAND_QT = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-qt', 'nexell-drm-mali-qt', '', d)}"

RDEPENDS_libcogl-path += " ${NX_WAYLAND_EGL_1} \
                           ${NX_WAYLAND_EGL_2} \
                           ${NX_WAYLAND_QT} \
                         "

RDEPENDS_libcogl += " ${NX_WAYLAND_EGL_1} \
                      ${NX_WAYLAND_EGL_2} \
                      ${NX_WAYLAND_QT} \
                    "

RDEPENDS_cogl-1.0-examples += " ${NX_WAYLAND_EGL_1} \
                                ${NX_WAYLAND_EGL_2} \
                                ${NX_WAYLAND_QT} \
                              "

RDEPENDS_libcogl-pango += " ${NX_WAYLAND_EGL_1} \
                            ${NX_WAYLAND_EGL_2} \
                            ${NX_WAYLAND_QT} \
                          "
                         
