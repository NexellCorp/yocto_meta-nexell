inherit kernel
inherit kernel-uboot
require recipes-kernel/linux/linux-yocto.inc

# externalsrc
inherit externalsrc
EXTERNALSRC = "${BSP_ROOT_DIR}/kernel-4.14"
EXTERNALSRC_BUILD = "${WORKDIR}/build"
EXTERNALSRC_SYMLINKS = ""

# This allows the kernel to be incrementally build within the shell
# and not dirty the source directory
KBUILD_OUTPUT = "${B}"
OE_TERMINAL_EXPORTS += "KBUILD_OUTPUT"

LINUX_VERSION_EXTENSION ?= "-nxp3220"

PV = "${LINUX_VERSION}+git${SRCPV}"

# The defconfig was created with make_savedefconfig so not all the configs are in place
KCONFIG_MODE="--alldefconfig"

do_configure_prepend() {
	cd "${S}"
	make distclean
}

# make boot.img
inherit classes/nexell-mkimage.bbclass
do_deploy_append () {
	make_ext4_bootimg ${D}/${KERNEL_IMAGEDEST}
}

# not execute tasks
#INITRAMFS_IMAGE = ""
do_bundle_initramfs[noexec] = "1"
do_rootfs[noexec] = "1"
