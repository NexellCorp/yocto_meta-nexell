DESCRIPTION = "OPTEE LINUXDRIVER"
inherit nexell-source-path-extension

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRCREV = "5fcce5d5800a60957141f1d963edfd199480bfcb"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_linuxdriver;protocol=git;branch=nexell"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

DEPENDS = "virtual/kernel"

PACKAGE_ARCH = "${MACHINE_ARCH}"

#COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p6818-avn-ref|s5p6818-bitminer-ref|s5p6818-kick-st)"
inherit module

KERNEL_MODULE_AUTOLOAD += "optee optee_armtz"

OPTEE_LINUXDRIVER_FLAGS = "ARCH=arm64 CROSS_COMPILE=${TARGET_PREFIX} LOCALVERSION= "

PATH_OPTEE_LINUXDRIVER = "${@env_setup(d,"optee-linuxdriver")}"
PATH_KBUILD_OUTPUT = "${@env_setup_kernel(d,"-standard-build")}"

deltask do_configure

do_compile() {
    export LDFLAGS="-O1 --hash-style=gnu --as-needed"

    if [ ${LOCAL_KERNEL_SOURCE_USING} = "true" ];then
#        oe_runmake -C ${STAGING_KERNEL_DIR} ${OPTEE_LINUXDRIVER_FLAGS} M=${PATH_OPTEE_LINUXDRIVER} clean
        oe_runmake -C ${STAGING_KERNEL_DIR} ${OPTEE_LINUXDRIVER_FLAGS} M=${PATH_OPTEE_LINUXDRIVER} modules
    else
#        oe_runmake -C ${BASE_WORKDIR}/temp_kernel_out ${OPTEE_LINUXDRIVER_FLAGS} M=${PATH_OPTEE_LINUXDRIVER} clean
        oe_runmake -C ${BASE_WORKDIR}/temp_kernel_out ${OPTEE_LINUXDRIVER_FLAGS} M=${PATH_OPTEE_LINUXDRIVER} modules
    fi
}

do_install() {
    install -d ${D}/lib/modules
    install -m 0755 ${B}/armtz/*.ko ${D}/lib/modules
    install -m 0755 ${B}/core/*.ko ${D}/lib/modules
}

FILES_${PN} += "/lib/modules"
