require nexell-uboot-common.inc

DESCRIPTION = "U-Boot for SVM S5P4418"

#SRC_URI += "file://"

COMPATIBLE_MACHINE = "s5p4418-svm-ref"

do_compile() {
    oe_runmake clean
    oe_runmake ARCH=arm s5p4418_svm_ref_defconfig
    oe_runmake ARCH=arm CROSS_COMPILE=${TARGET_PREFIX} LDFLAGS="" CC="$CC"
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