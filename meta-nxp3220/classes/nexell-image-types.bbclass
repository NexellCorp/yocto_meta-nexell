# nexell image types

inherit nexell-mkimage

do_image_multiubi2[depends] += "mtd-utils-native:do_populate_sysroot"

IMAGE_CMD_multiubi2 () {
	if [ -z ${FLASH_PAGE_SIZE} ] || [ -z ${FLASH_BLOCK_SIZE} ] ||
	   [ -z ${FLASH_DEVICE_SIZE} ]; then
		bbfatal "Not defined 'FLASH_PAGE_SIZE or FLASH_BLOCK_SIZE or FLASH_DEVICE_SIZE'"
	fi

	local images="$(echo ${MKUBIFS_ARGS_rootfs} | awk -F',' '{ print NF }')"
	local i=1

	while [ $i -le $images ];
	do
	        args="$(echo $(echo ${MKUBIFS_ARGS_rootfs}| cut -d',' -f $i))"
		args="$args -p ${FLASH_PAGE_SIZE} -b ${FLASH_BLOCK_SIZE} -c ${FLASH_DEVICE_SIZE}"

		make_ubi_image ${args}

	        i=`expr $i + 1`
	done;
}
