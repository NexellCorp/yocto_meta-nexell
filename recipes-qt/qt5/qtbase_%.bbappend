COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p4418-avn-ref|s5p4418-navi-ref|s5p6818-avn-ref)"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

PACKAGECONFIG = " \
    ${PACKAGECONFIG_RELEASE} \
    ${PACKAGECONFIG_DEFAULT} \
    ${PACKAGECONFIG_OPENSSL} \
    ${PACKAGECONFIG_FB} \
    ${PACKAGECONFIG_FONTS} \
    ${PACKAGECONFIG_SYSTEM} \
    ${PACKAGECONFIG_MULTIMEDIA} \
    ${PACKAGECONFIG_DISTRO} \
"

EXTRA_OECONF += "'-I${STAGING_DIR_TARGET}/usr/include/interface/vcos/pthreads/' \
                 '-I${STAGING_DIR_TARGET}/usr/include/interface/vmcs_host/linux/'" 

