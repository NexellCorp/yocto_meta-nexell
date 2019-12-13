# Generic SWUpdate artifacts generation

inherit nexell-mkimage

DEPENDS += "android-tools-native"
DEPENDS += "mtd-utils-native"
DEPENDS += "${@ 'openssl-native' if d.getVar('SWU_SIGN_PASSWORD') else ''}"

RECOVERY_IMAGE ?= "recovery.uinitrd"

make_recovery_boot_image() {
	local recovery_img=$1
	local deploy_dir=${DEPLOY_DIR_IMAGE}
	local boot_dir=${deploy_dir}/${PART_BOOT_LABEL}
	local boot_img=${deploy_dir}/${PART_BOOT_IMAGE}

	if [ -z "${recovery_img}" ] || [ ! -f "${deploy_dir}/${recovery_img}" ]; then
		echo "WARNING: NOT FOUND '${deploy_dir}/${recovery_img}'"
		return
	fi

	mkdir -p ${boot_dir}

	# Copy and rename to '${RECOVERY_IMAGE}'
	# The '${RECOVERY_IMAGE}' is reserved with u-boot's boot command for
	# recovery boot.
	cp ${deploy_dir}/${recovery_img} ${boot_dir}/${RECOVERY_IMAGE}

	if echo "$(file -b ${boot_img})" | grep -q -iE "android|ext4"; then
		if [ -z ${PART_BOOT_SIZE} ]; then
			echo "WARNING: NOT DEFINED 'PART_BOOT_SIZE'"
			return
		fi

		make_ext4_image ${PART_BOOT_LABEL} ${PART_BOOT_SIZE} ${boot_dir} ${boot_img}
	fi

	if echo "$(file -b ${boot_img})" | grep -q -iE "UBI"; then
		if [ -z ${FLASH_PAGE_SIZE} ] || [ -z ${FLASH_BLOCK_SIZE} ] ||
		   [ -z ${FLASH_DEVICE_SIZE} ]; then
			bbfatal "Not defined 'FLASH_PAGE_SIZE or FLASH_BLOCK_SIZE or FLASH_DEVICE_SIZE'"
		fi

		local images="$(echo ${MKUBIFS_ARGS_boot} | awk -F',' '{ print NF }')"
		local i=1

		while [ $i -le $images ];
		do
		        args="$(echo $(echo ${MKUBIFS_ARGS_boot}| cut -d',' -f $i))"
			args="$args -p ${FLASH_PAGE_SIZE} -b ${FLASH_BLOCK_SIZE} -c ${FLASH_DEVICE_SIZE}"

			make_ubi_image ${args}

		        i=`expr $i + 1`
		done;
	fi
}

SWU_SWDESCRIPTION ?= ""
SWU_SIGN_PASSWORD ?= ""
SWU_SIGN_PRIVATE_KEY ?= ""
SWU_SIGN_PUBLIC_KEY ?= ""

make_swupdate_image() {
	local swuname=$1
	local swudesc=$(basename ${SWU_SWDESCRIPTION})
	local swutool=${DEPLOY_DIR_IMAGE}/${SWU_TOOL_IMAGE_GEN}
	local option=""

	if [ ! -e "${SWU_SWDESCRIPTION}" ]; then
		echo "ERROR: NOT Defined 'SWU_SWDESCRIPTION'"
		exit 1
	fi

	# copy sw-description
	install -m 644 ${SWU_SWDESCRIPTION} ${DEPLOY_DIR_IMAGE}/

	# copy SWUpdate script files
	if [ ! -z "${SWU_SCRIPT_FILES}" ]; then
		install ${SWU_SCRIPT_FILES} ${DEPLOY_DIR_IMAGE}
	fi

	cd ${DEPLOY_DIR_IMAGE}

	# set build options
	#
	[ ! -z "${swuname}" ] && option="${option} -o ${swuname}";

	# set signing options
	#
	[ ! -z "${SWU_SIGN_ENABLE}" ] && [ ${SWU_SIGN_ENABLE} == true ] && option="${option} -s";

	if [ ! -z ${SWU_SIGN_PASSWORD} ]; then
	 	option="${option} -p ${SWU_SIGN_PASSWORD}";
	fi

	if [ ! -z "${SWU_SIGN_PRIVATE_KEY}" ]; then
		install -m 644 ${SWU_SIGN_PRIVATE_KEY} ${DEPLOY_DIR_IMAGE}/swu.private.key
		option="${option} -k ${DEPLOY_DIR_IMAGE}/swu.private.key";
	fi

	if [ ! -z "${SWU_SIGN_PUBLIC_KEY}" ]; then
		install -m 644 ${SWU_SIGN_PUBLIC_KEY} ${DEPLOY_DIR_IMAGE}/swu.public.key
	fi

	swudesc=${DEPLOY_DIR_IMAGE}/${swudesc}

	# swupdate build command
	echo "SWU: ${swutool} -f ${swudesc} -d ${DEPLOY_DIR_IMAGE} ${option}"

	${swutool} -f ${swudesc} -d ${DEPLOY_DIR_IMAGE} ${option}
}

make_recovery_image() {
	# copy recovery rootfs image to boot.img
	make_recovery_boot_image "${SWU_RECOVERY_ROOTFS_IMAGE}"

	# make recovery image with swupdate
	# $1: output swu image name
	make_swupdate_image "$1"
}
