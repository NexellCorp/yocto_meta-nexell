require u-boot-nexell.inc

DESCRIPTION = "U-Boot for NEXELL"

EXTRA_OEMAKE = 'ARCH=arm CROSS_COMPILE=${TARGET_PREFIX} CC="${TARGET_PREFIX}gcc ${TOOLCHAIN_OPTIONS}" V=1'
EXTRA_OEMAKE += 'HOSTCC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}"'
DEPENDS = "dtc-native bc-native"

#INSANE_SKIP_${PN} = "already-stripped"
INSANE_SKIP_${PN} = "ldflags"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"

do_compile() {
    oe_runmake clean
    oe_runmake ${UBOOT_DEFCONFIG}
    oe_runmake
    oe_runmake env
}

do_install() {
    cp `find . -name "env_common.o"` copy_env_common.o
    ${TARGET_PREFIX}objcopy -O binary --only-section=.rodata.default_environment `find . -name "copy_env_common.o"`
    tr '\0' '\n' < copy_env_common.o > default_envs.txt
    cp default_envs.txt default_envs.txt.orig

	install -d ${D}${base_sbindir}
    install -m 0755 ${S}/tools/env/fw_printenv ${D}${base_sbindir}/fw_printenv
    install -m 0755 ${S}/tools/env/fw_printenv ${D}${base_sbindir}/fw_setenv

    install -d ${D}${sysconfdir}
    install -m 0644 ${S}/tools/env/fw_env.config ${D}${sysconfdir}/fw_env.config

}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/u-boot.bin ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/default_envs.txt ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install
