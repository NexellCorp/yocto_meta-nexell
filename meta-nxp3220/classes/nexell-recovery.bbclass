# Generic SWUpdate artifacts generation

inherit nexell-mkimage

DEPENDS += "android-tools-native"
DEPENDS += "mtd-utils-native"
DEPENDS += "${@ 'openssl-native' if d.getVar('SWU_SIGN_PASSWORD') else ''}"

RECOVERY_IMAGE ?= "recovery.uinitrd"

make_recovery_boot_image() {
	local S_DIR=$1 RECOVERY_ROOT=$2
	local D_DIR=${DEPLOY_DIR_IMAGE}/${PART_BOOT_LABEL}
	local IMAGE=${DEPLOY_DIR_IMAGE}/${PART_BOOT_IMAGE}

	if [ -z "${RECOVERY_ROOT}" ] || [ ! -f "${S_DIR}/${RECOVERY_ROOT}" ]; then
		echo "WARNING: NOT FOUND '${S_DIR}/${RECOVERY_ROOT}'"
		return
	fi

	mkdir -p ${D_DIR}
	cp ${S_DIR}/${RECOVERY_ROOT} ${D_DIR}/${RECOVERY_IMAGE}

	if echo "$(file -b ${IMAGE})" | grep -q -iE "android|ext4"; then
		if [ -z ${PART_BOOT_SIZE} ]; then
			echo "WARNING: NOT DEFINED 'PART_BOOT_SIZE'"
			return
		fi

		make_ext4_image ${PART_BOOT_LABEL} ${PART_BOOT_SIZE} ${D_DIR} ${IMAGE}
	fi

	if echo "$(file -b ${IMAGE})" | grep -q -iE "UBI"; then
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
	local name=$1
	local SWUDESC=$(basename ${SWU_SWDESCRIPTION})
	local SWUTOOL=${DEPLOY_DIR_IMAGE}/${SWU_TOOL_IMAGE_GEN}
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
	[ ! -z "${name}" ] && option="${option} -o ${name}";

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

	SWUDESC=${DEPLOY_DIR_IMAGE}/${SWUDESC}

	# swupdate build command
	echo "SWU: ${SWUTOOL} -f ${SWUDESC} -d ${DEPLOY_DIR_IMAGE} ${option}"

	${SWUTOOL} -f ${SWUDESC} -d ${DEPLOY_DIR_IMAGE} ${option}
}

make_recovery_image() {
	# copy recovery rootfs image to boot.img
	make_recovery_boot_image "${DEPLOY_DIR_IMAGE}" "${SWU_RECOVERY_ROOTFS_IMAGE}"

	# make recovery image with swupdate
	make_swupdate_image "$1"
}
