DESCRIPTION = "OPTEE BUILD"
inherit nexell-optee-preprocess
include optee-secure.inc

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README.md;md5=871e3be979e189da85a03fe6ba47bbe2"

SRCREV = "16ce9bbae832d1e08430e77f2e34f9fe7accb3a6"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_build;protocol=git;branch=artik \
           file://0001-optee-build-compile-error-patch.patch"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
OPTEE_BUILD_TARGET_MACHINE=""

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor)"

inherit pkgconfig
DEPENDS = "arm-trusted-firmware l-loader optee-os optee-client optee-test s5p6818-artik710-raptor-uboot s5p6818-artik710-raptor-bl1"

TOOLCHAIN_32 = "${BASE_WORKDIR}/gcc-linaro-4.9-2014.11-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-"
TOOLCHAIN_32_BIN_PATH = "${BASE_WORKDIR}/gcc-linaro-4.9-2014.11-x86_64_arm-linux-gnueabihf/bin"
TOOLCHAIN_32_LIB_PATH = "${BASE_WORKDIR}/gcc-linaro-4.9-2014.11-x86_64_arm-linux-gnueabihf/lib"
TOOLCHAIN_32_LIB_FLAGS = "-L${TOOLCHAIN_32_LIB_PATH}"

COMMON_FLAGS = "PLAT_DRAM_SIZE=1024 PLAT_UART_BASE=0xc00a3000 ${SECURE-OPTEE} CROSS_COMPILE=${TARGET_PREFIX} CROSS_COMPILE32=${TOOLCHAIN_32}"

PATH_OPTEE_BUILD = "${@env_setup(d,"optee-build")}"
PATH_OPTEE_OS = "${@env_setup(d,"optee-os")}"
PATH_OPTEE_CLIENT = "${@env_setup(d,"optee-client")}"
PATH_OPTEE_LINUXDRIVER = "${@env_setup(d,"optee-linuxdriver")}"
PATH_OPTEE_TEST = "${@env_setup(d,"optee-test")}"
PATH_ATF = "${@env_setup(d,"arm-trusted-firmware")}"
PATH_L-LOADER = "${@env_setup(d,"l-loader")}"
PATH_U-BOOT = "${@env_setup(d,"${OPTEE_BUILD_TARGET_MACHINE}-uboot")}"
PATH_BL1 = "${@env_setup(d,"${OPTEE_BUILD_TARGET_MACHINE}-bl1")}"
PATH_KERNEL_SRC = "${@env_setup(d,"kernel-source")}"
PATH_KERNEL_BUILD = "${@env_setup(d,"kernel-build-artifacts")}"

python do_make_source_dir_pathfile() {
    import commands
    
    base_workdir = bb.data.getVar("BASE_WORKDIR", d, True)
    multimach_host_sys = bb.data.getVar("MULTIMACH_HOST_SYS", d, True)
    kernel_src_path = bb.data.getVar("STAGING_KERNEL_DIR", d, True)
    kernel_build_path = bb.data.getVar("STAGING_KERNEL_BUILDDIR", d, True)
    
    pv = bb.data.getVar("PV", d, True)
    pr = bb.data.getVar("PR", d, True)

    recipes_optee_paths = ["optee-build", "optee-os", "optee-client", "optee-linuxdriver", "optee-test"]
    recipes_lloader_paths = ["l-loader"]
    recipes_atf_paths = ["arm-trusted-firmware"]
    recipes_others = ["${OPTEE_BUILD_TARGET_MACHINE}-bl1", "${OPTEE_BUILD_TARGET_MACHINE}-uboot", "linux-${OPTEE_BUILD_TARGET_MACHINE}"]
    
    recipes_all = recipes_optee_paths + recipes_lloader_paths + recipes_atf_paths + recipes_others

    commands.getoutput('echo %s > %s/source_dir_path.txt' % ("For Yocto ${MACHINE_ARCH} optee build",base_workdir))
    for dirname in recipes_all :
        if dirname == "linux-${OPTEE_BUILD_TARGET_MACHINE}" :
            commands.getoutput('echo %s >> %s/source_dir_path.txt' % (kernel_src_path, base_workdir))
            commands.getoutput('echo %s >> %s/source_dir_path.txt' % (kernel_build_path, base_workdir))
        else :
            temp = base_workdir + "/" + multimach_host_sys + "/" + dirname + "/" + pv + "-" + pr + "/git"	    
            commands.getoutput('echo %s >> %s/source_dir_path.txt' % (temp, base_workdir))
}

do_make_symlink() {
    ln -sf "${PATH_OPTEE_OS}" ${S}/optee_os
    ln -sf "${PATH_OPTEE_CLIENT}" ${S}/optee_client
    ln -sf "${PATH_OPTEE_LINUXDRIVER}" ${S}/optee_linuxdriver
    ln -sf "${PATH_OPTEE_TEST}" ${S}/optee_test
    ln -sf "${PATH_ATF}" ${S}/arm-trusted-firmware
    ln -sf "${PATH_L-LOADER}" ${S}/l-loader
    ln -sf "${PATH_U-BOOT}" ${S}/u-boot-artik7
    ln -sf "${PATH_BL1}" ${S}/bl1-artik7
    ln -sf "${PATH_KERNEL_BUILD}" ${S}/linux-artik7
}

do_compile() {
    export PATH=${TOOLCHAIN_32_BIN_PATH}:$PATH
    export LDFLAGS=""
    
    oe_runmake -f ${WORKDIR}/git/Makefile clean
    oe_runmake -f ${WORKDIR}/git/Makefile build-bl1 -j8
    oe_runmake -f ${WORKDIR}/git/Makefile ${COMMON_FLAGS} build-lloader -j8

    oe_runmake -f ${WORKDIR}/git/Makefile ${COMMON_FLAGS} build-bl32 -j8

    oe_runmake -f ${WORKDIR}/git/Makefile ${COMMON_FLAGS} build-fip -j8
    oe_runmake -f ${WORKDIR}/git/Makefile ${COMMON_FLAGS} build-fip-loader -j8
    oe_runmake -f ${WORKDIR}/git/Makefile ${COMMON_FLAGS} build-fip-secure -j8
    oe_runmake -f ${WORKDIR}/git/Makefile ${COMMON_FLAGS} build-fip-nonsecure -j8

    oe_runmake -f ${WORKDIR}/git/Makefile ${COMMON_FLAGS} build-optee-client
    oe_runmake -f ${WORKDIR}/git/Makefile ${COMMON_FLAGS} OPTEE_CLIENT_EXPORT="${PATH_OPTEE_CLIENT}/out/export" CFLAGS="" build-optee-test
    oe_runmake -f ${WORKDIR}/git/Makefile ${COMMON_FLAGS} build-singleimage -j8
}

do_install() {
    mkdir -p ${D}/usr/bin
    mkdir -p ${D}/usr/lib
    rm -rf ${D}/lib/optee_armtz
    mkdir -p ${D}/lib/optee_armtz

    install -d ${D}/usr/bin
    install -m 0755 ${PATH_OPTEE_CLIENT}/out/export/bin/tee-supplicant ${D}/usr/bin
#    install -m 0755 ${PATH_OPTEE_TEST}/out/xtest/xtest ${D}/usr/bin

    install -d ${D}/usr/lib
    install -m 0755 ${PATH_OPTEE_CLIENT}/out/export/lib/* ${D}/usr/lib        

    cd ${PATH_OPTEE_TEST}/out/ta
#    find . -name "*.ta" -exec cp {} ${D}/lib/optee_armtz \;
#    chmod 444 ${D}/lib/optee_armtz/*.ta    
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/optee_build/result/fip-loader.bin ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/optee_build/result/fip-nonsecure.bin ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/optee_build/result/fip-secure.bin ${DEPLOY_DIR_IMAGE}
}

FILES_${PN} += " \
        ${bindir}/* \
        ${libdir}/* \
        /lib/optee_armtz \
        "

addtask make_source_dir_pathfile before do_configure
addtask make_symlink after do_configure before do_compile
addtask deploy after do_install
