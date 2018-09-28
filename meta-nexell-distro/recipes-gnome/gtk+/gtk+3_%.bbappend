NX_WAYLAND_EGL_1 = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-fb', 'nexell-drm-mali-sdl-fb', '', d)}"
NX_WAYLAND_EGL_2 = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-wayland', 'nexell-drm-mali-sdl-wayland', '', d)}"
NX_WAYLAND_QT = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-qt', 'nexell-drm-mali-qt', '', d)}"

DEPENDS += " ${NX_WAYLAND_EGL_1} \
             ${NX_WAYLAND_EGL_2} \
             ${NX_WAYLAND_QT} \
           "

RDEPENDS_${PN} += \
                  " ${NX_WAYLAND_EGL_1} \
                    ${NX_WAYLAND_EGL_2} \
                    ${NX_WAYLAND_QT} \
                  "
RDEPENDS_gtk3-immodule-ipa += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-immodule-thai += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-immodule-cyrillic-translit += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-immodule-viqr += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-immodule-am-et += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-immodule-cedilla += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-immodule-inuktitut += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-printbackend-lpr += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-immodule-ti-et += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-immodule-multipress += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-printbackend-file += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
RDEPENDS_gtk3-immodule-ti-er += \
                 " ${NX_WAYLAND_EGL_1} \
                   ${NX_WAYLAND_EGL_2} \
                   ${NX_WAYLAND_QT} \
                 "
