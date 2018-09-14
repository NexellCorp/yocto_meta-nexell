require nexell-uboot-common.inc

DESCRIPTION = "U-Boot for CONVERGENCE_SVMC S5P4418"

#SRC_URI += "file://"

COMPATIBLE_MACHINE = "s5p4418-convergence-svmc"

do_compile() {
    oe_runmake clean
    oe_runmake ARCH=arm s5p4418_convergence_svmc_defconfig
    oe_runmake ARCH=arm CROSS_COMPILE=${TARGET_PREFIX} LDFLAGS="" CC="$CC"
}
# oe_runmake ARCH=arm CROSS_COMPILE=${TARGET_PREFIX} LDFLAGS="" CC="$CC" QUICKBOOT=1
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
