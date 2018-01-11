PROVIDES_remove = " virtual/libgles1 virtual/libgles2 virtual/libopenvg "
PROVIDES_remove += "${@bb.utils.contains('DISTRO_FEATURES', 'x11', '', 'virtual/egl', d)}"

BACKEND = "${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'x11', 'none', d)}"

do_install_append () {
    if [ ${BACKEND} == "x11" ]; then
       echo "nothing"
    else
       rm -f ${D}/${libdir}/libEGL*
    fi
    rm -f ${D}/${libdir}/libGLESv1_CM.*
    rm -f ${D}/${libdir}/libGLESv2.*
    rm -f ${D}/${libdir}/libgbm*
    rm -f ${D}/${libdir}/libwayland-egl*
}
