inherit kernel
inherit kernel-uboot
require recipes-kernel/linux/linux-yocto.inc

# externalsrc
# EXTERNALSRC_BUILD = "${WORKDIR}/build"
# EXTERNALSRC_BUILD = "${EXTERNALSRC}"
inherit externalsrc
EXTERNALSRC = "${BSP_ROOT_DIR}/kernel-4.14"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""

# This allows the kernel to be incrementally build within the shell
# and not dirty the source directory
KBUILD_OUTPUT = "${B}"
OE_TERMINAL_EXPORTS += "KBUILD_OUTPUT"

LINUX_VERSION_EXTENSION ?= "-nxp3220"

PV = "${LINUX_VERSION}+git${SRCPV}"

# The defconfig was created with make_savedefconfig so not all the configs are in place
KCONFIG_MODE="--alldefconfig"

do_configure_prepend () {
    if [ "${EXTERNALSRC}" != "${EXTERNALSRC_BUILD}" ]; then
	oe_runmake -C ${S} distclean
    else
        file=${B}/.kernel_defconfig
	if [ -e ${file} ] && [ -d ${WORKDIR} ]; then
	    conf=$(cat ${file})
	    if [ "${conf}" != "${KBUILD_DEFCONFIG}" ]; then
		rm ${file}; echo ${KBUILD_DEFCONFIG} >> ${file};
	        oe_runmake -C ${S} distclean
	    fi
	else
	    echo ${KBUILD_DEFCONFIG} >> ${file};
	    oe_runmake -C ${S} distclean
	fi
    fi

    if [ ! -f ${B}/.config ]; then
	oe_runmake ARCH=arm -C ${S} O=${B} ${KBUILD_DEFCONFIG}
    fi
}

do_clean_distclean () {
    if [ "${EXTERNALSRC}" = "${EXTERNALSRC_BUILD}" ]; then
	oe_runmake -C ${S} distclean
    fi
}
addtask do_clean_distclean before do_cleansstate after do_clean

# symblolic link work-shared/<machine>/kernel-source
python do_symlink_staging_dir () {
    s = d.getVar("S")
    if s[-1] == '/':
        # drop trailing slash, so that os.symlink(kernsrc, s) doesn't use s as directory name and fail
        s=s[:-1]
    kernsrc = d.getVar("STAGING_KERNEL_DIR")
    if s != kernsrc:
        bb.utils.mkdirhier(kernsrc)
        bb.utils.remove(kernsrc, recurse=True)
        if d.getVar("EXTERNALSRC"):
            # With EXTERNALSRC S will not be wiped so we can symlink to it
            os.symlink(s, kernsrc)
        else:
            import shutil
            shutil.move(s, kernsrc)
            os.symlink(kernsrc, s)
}
addtask do_symlink_staging_dir after do_unpack before do_patch do_configure

# make boot.img
inherit classes/nexell-mkimage.bbclass
do_deploy_append () {
	make_ext4_bootimg ${D}/${KERNEL_IMAGEDEST}
}

# To ensure that the board config has changed, always run the config command
do_configure[nostamp] = "1"

# not execute tasks
do_bundle_initramfs[noexec] = "1"
do_rootfs[noexec] = "1"
