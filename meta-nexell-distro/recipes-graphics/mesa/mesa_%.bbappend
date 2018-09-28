PROVIDES_remove = " virtual/libgles1 virtual/libgles2 virtual/libopenvg "
PROVIDES_remove += "${@bb.utils.contains('DISTRO_FEATURES', 'x11', '', 'virtual/egl', d)}"

BACKEND = "${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'x11', 'none', d)}"

do_install_append () {
    # Drivers never need libtool .la files
    rm -f ${D}${libdir}/dri/*.la
    rm -f ${D}${libdir}/egl/*.la
    rm -f ${D}${libdir}/gallium-pipe/*.la
    rm -f ${D}${libdir}/gbm/*.la

    # it was packaged in libdricore9.1.3-1 and preventing upgrades when debian.bbclass was used 
    rm -f ${D}${sysconfdir}/drirc
    chrpath --delete ${D}${libdir}/dri/*_dri.so || true

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
