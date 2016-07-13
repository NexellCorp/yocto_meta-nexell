COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p4418-avn-ref|s5p4418-navi-ref)"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append = "file://0001-fix-rasp-build.patch"

PACKAGECONFIG = " \
    ${PACKAGECONFIG_RELEASE} \
    ${PACKAGECONFIG_DEFAULT} \
    ${PACKAGECONFIG_OPENSSL} \
    ${PACKAGECONFIG_GL} \
    ${PACKAGECONFIG_FB} \
    ${PACKAGECONFIG_FONTS} \
    ${PACKAGECONFIG_SYSTEM} \
    ${PACKAGECONFIG_MULTIMEDIA} \
    ${PACKAGECONFIG_DISTRO} \
    gles2 icu alsa \
"

EXTRA_OECONF += "'-I${STAGING_DIR_TARGET}/usr/include/interface/vcos/pthreads/' \
                 '-I${STAGING_DIR_TARGET}/usr/include/interface/vmcs_host/linux/'" 
