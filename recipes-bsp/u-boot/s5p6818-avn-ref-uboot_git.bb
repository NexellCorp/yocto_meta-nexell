require nexell-uboot-common.inc

vDESCRIPTION = "U-Boot for avn-ref"

#SRC_URI += "file://"

COMPATIBLE_MACHINE = "s5p6818-avn-ref"

do_compile() {
    local config="s5p6818_arm64_avn_ref"

    oe_runmake clean
    oe_runmake ${config}_defconfig
    oe_runmake CROSS_COMPILE=aarch64-poky-linux- LDFLAGS="" CC="$CC"
}

do_install() {
    cp `find . -name "env_common.o"` copy_env_common.o
    ${TARGET_PREFIX}objcopy -O binary --only-section=.rodata.default_environment `find . -name "copy_env_common.o"`
    tr '\0' '\n' < copy_env_common.o > default_envs.txt
    cp default_envs.txt default_envs.txt.orig
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/u-boot.bin ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/default_envs.txt ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install