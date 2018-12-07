DESCRIPTION = "OPTEE LINUXDRIVER"
inherit nexell-source-path-extension

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRCREV = "5fcce5d5800a60957141f1d963edfd199480bfcb"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_linuxdriver;protocol=git;branch=nexell"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

DEPENDS = "virtual/kernel gcc-linaro-4.9-2015.05-x86-64-aarch64-linux-gnu"

PACKAGE_ARCH = "${MACHINE_ARCH}"

#COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p6818-avn-ref|s5p6818-kick-st)"
inherit module

KERNEL_MODULE_AUTOLOAD += "optee optee_armtz"

TOOLCHAIN_64 = "${RECIPE_SYSROOT}${datadir}/${TOOLCHAIN_AARCH64_PREBUILT}/bin/aarch64-linux-gnu-"

OPTEE_LINUXDRIVER_FLAGS = "ARCH=arm64 CROSS_COMPILE=${TARGET_PREFIX} LOCALVERSION= "

PATH_OPTEE_LINUXDRIVER ?= "${@env_setup(d,"optee-linuxdriver")}"
PATH_KBUILD_OUTPUT = "${@env_setup_kernel(d,"-standard-build")}"

#deltask do_configure
do_configure() {
    if [ -n "${CONFIGURESTAMPFILE}" -a -e "${CONFIGURESTAMPFILE}" ]; then
        if [ "`cat ${CONFIGURESTAMPFILE}`" != "${BB_TASKHASH}" ]; then
            cd ${S}
            # if [ "0" != "1" -a \( -e Makefile -o -e makefile -o -e GNUmakefile \) ]; then
            #     oe_runmake clean
            # fi
            find ${S} -ignore_readdir_race -name \*.la -delete
        fi
    fi

    if [ -n "${CONFIGURESTAMPFILE}" ]; then
        mkdir -p `dirname ${CONFIGURESTAMPFILE}`
        echo ${BB_TASKHASH} > ${CONFIGURESTAMPFILE}
    fi
}

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

do_buildclean() {
    if [ "${EXTERNALSRC}" != "" ];then
        if [ -e Makefile -o -e makefile -o -e GNUmakefile ]; then
           rm -f oe-workdir oe-logs
           # if [ "0" != "1" ]; then
           #     oe_runmake clean || die "make failed"
           # fi
        else
           bbnote "nothing to do - no makefile found"
        fi
    fi
}

FILES_${PN} += "/lib/modules"