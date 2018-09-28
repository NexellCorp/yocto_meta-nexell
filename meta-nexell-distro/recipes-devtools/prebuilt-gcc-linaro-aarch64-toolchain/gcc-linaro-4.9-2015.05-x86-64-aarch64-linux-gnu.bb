DESCRIPTION = "gcc-linaro aarch64 external toolchain"
LICENSE="GPLv2"

LIC_FILES_CHKSUM = "file://.gitattributes;md5=a92c6c0fbde1f2ede1d1c670bca72401"

SRCREV="a8fc18239bdb6789e1a790681487eff109e62ea9"

SRC1 = "gcc-linaro-4.9-2015.05-x86_64_aarch64-linux-gnu1"
SRC2 = "gcc-linaro-4.9-2015.05-x86_64_aarch64-linux-gnu2"

SRC_URI = "git://github.com/NexellBackUp/nexell_nougat_toolchain;branch=nougat-dev;protocols=https"

SRC_URI[md5sum] = "afc1981b69a5c30e3eb87f8198e5dcf2"
SRC_URI[sha256sum] = "c7a66128a0926a512c7c47df461ecc04f72ded96a973a09c15171d8b498793ed"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_configure() {
    :
}
do_compile() {
    :
}    
do_install () {
    # if [ ! -d ${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT} ];then
    #     install -d ${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT}
    # fi

    # if [ ! -d "${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT}/aarch64-linux-gnu" ];then
    #     tar -xvf ${S}/${TOOLCHAIN_AARCH64_PREBUILT}/${SRC1}.tar.xz -C ${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT}
    #     tar -xvf ${S}/${TOOLCHAIN_AARCH64_PREBUILT}/${SRC2}.tar.xz -C ${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT}

    #     mv -f ${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT}/${SRC1}/* ${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT}
    #     mv -f ${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT}/${SRC2}/* ${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT}

    #     rm -rf ${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT}/${SRC1}
    #     rm -rf ${BASE_WORKDIR}/${TOOLCHAIN_AARCH64_PREBUILT}/${SRC2}
    # fi

    install -d ${D}${datadir}/${TOOLCHAIN_AARCH64_PREBUILT}

    tar -xvf ${S}/${TOOLCHAIN_AARCH64_PREBUILT}/${SRC1}.tar.xz -C ${D}${datadir}/${TOOLCHAIN_AARCH64_PREBUILT}
    tar -xvf ${S}/${TOOLCHAIN_AARCH64_PREBUILT}/${SRC2}.tar.xz -C ${D}${datadir}/${TOOLCHAIN_AARCH64_PREBUILT}

    mv -f ${D}${datadir}/${TOOLCHAIN_AARCH64_PREBUILT}/${SRC1}/* ${D}${datadir}/${TOOLCHAIN_AARCH64_PREBUILT}
    mv -f ${D}${datadir}/${TOOLCHAIN_AARCH64_PREBUILT}/${SRC2}/* ${D}${datadir}/${TOOLCHAIN_AARCH64_PREBUILT}
}

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"

INSANE_SKIP_${PN} = "installed-vs-shipped already-stripped license-checksum"
INSANE_SKIP_${PN}-dbg = "installed-vs-shipped already-stripped"
