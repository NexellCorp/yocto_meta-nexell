# Generate Recovery Root filesystem

SUMMARY = "Recovery Root filesystem as rescue system and Generate recovery image"
DESCRIPTION = "Recovery Root FS in rescue mode and Generate recovery image"
LICENSE = "CLOSED"

#
# for swupdate rootfs
#
inherit nexell-mkimage nexell-recovery
require recipes-extended/images/swupdate-image.inc

do_recovery_image() {
	# generate swu image
	make_recovery_image "${SWU_UPDATE_SWU_IMAGE}"
}
addtask recovery_image after do_image_complete before do_build

# depends to generate swu image
do_recovery_image[depends] += " virtual/kernel:do_deploy"
do_recovery_image[depends] += " ${INITRAMFS_IMAGE}:do_rootfs_image"
do_recovery_image[depends] += " ${INITRAMFS_IMAGE}:do_data_image"
do_recovery_image[depends] += " ${INITRAMFS_IMAGE}:do_misc_image"

# always run task
do_recovery_image[nostamp] = "1"

# CONVERSION_CMD_u-boot: convert cpio.gz to uinitrd with mkimage tool in u-boot
IMAGE_FSTYPES += "cpio.gz.u-boot"

DEPENDS_append = " update-rc.d-native"

IMAGE_INSTALL_append = " \
	swupdate \
	swupdate-tools \
	swupdate-recovery \
	udev \
	util-linux-agetty \
	"

# for test
IMAGE_INSTALL_append = " android-tools"

agetty_autologin_inittab () {
	local rootdir=${IMAGE_ROOTFS}

        sed -e 's/^S2:12345.*/S2:12345:respawn:\/sbin\/agetty ${AGETTY_AUTOLOGIN_INITTAB_ARGUMENTS}/' \
                "${rootdir}${sysconfdir}/inittab" > "${rootdir}${sysconfdir}/inittab.swupdate.autologin"

        rm ${rootdir}${sysconfdir}/inittab
        mv ${rootdir}${sysconfdir}/inittab.swupdate.autologin ${rootdir}${sysconfdir}/inittab
}

postprocess_recovery_image () {
	local rootdir=${IMAGE_ROOTFS}

	mkdir -p ${rootdir}/misc

	# mount /tmp with tmpfs : recovery image is initrd so this is not necessary.
	# echo "tmpfs /tmp tmpfs mode=1777,nosuid,nodev 0 0" >> ${rootdir}${sysconfdir}/fstab

	# misc partition add to /etc/fstab for ext4
	if [ ! -z "${PART_MISC_SIZE}" ] && [ ! -z "${PART_MISC_NODE}" ]; then
		echo "${PART_MISC_NODE} /misc ext4 noatime,nosuid,nodev,nomblk_io_submit,errors=panic wait,check" \
			>> ${rootdir}${sysconfdir}/fstab
	fi

	# misc partition add to /etc/fstab for ubi
	if [ ! -z "${MKUBIFS_ARGS_misc}" ] && [ ! -z "${PART_MISC_NODE}" ]; then
		echo "${PART_MISC_NODE} /misc ubifs defaults,auto 0 0" \
			>> ${rootdir}${sysconfdir}/fstab
	fi

	# remove not necessary services
	#
	update-rc.d -r ${rootdir} -f psplash.sh remove

	# remove not necessary udev rules
	#
	rm ${rootdir}/lib/udev/rules.d/*

	if [ -f ${rootdir}${sysconfdir}/udev/rules.d/local.rules ]; then
		rm ${rootdir}${sysconfdir}/udev/rules.d/local.rules
	fi
	if [ -f ${rootdir}${sysconfdir}/udev/rules.d/touchscreen.rules ]; then
		rm ${rootdir}${sysconfdir}/udev/rules.d/touchscreen.rules
	fi
}

ROOTFS_POSTPROCESS_COMMAND += "agetty_autologin_inittab;"
ROOTFS_POSTPROCESS_COMMAND += "postprocess_recovery_image;"
