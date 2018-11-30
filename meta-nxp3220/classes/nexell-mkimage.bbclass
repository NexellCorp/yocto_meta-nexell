make_ext4_image() {
	PART_LABEL=$1
	PART_SIZE=$2
	PART_DIR=$3
	OUTPUT_IMAGE=$4

	echo "MAKE EXT4: ${PART_LABEL} (${PART_SIZE}) ${PART_DIR} -> ${OUTPUT_IMAGE}"
        dd if=/dev/zero of=${OUTPUT_IMAGE} seek=${PART_SIZE} bs=1 count=0
 	mkfs.ext4 -L ${PART_LABEL} -d ${PART_DIR} ${OUTPUT_IMAGE}
}

PART_BOOT_LABEL ?= "boot"
PART_BOOT_SIZE ?= ""

make_ext4_bootimg() {
	S_DIR=$1
	D_DIR=${DEPLOY_DIR_IMAGE}/boot
	IMAGE=${DEPLOY_DIR_IMAGE}/boot.img

	if [ -z ${PART_BOOT_SIZE} ]; then
		return
	fi

	mkdir -p ${D_DIR}

	if [ -e ${S_DIR}/${KERNEL_DEVICETREE} ]; then
		DTBFILE=${S_DIR}/${KERNEL_DEVICETREE}
	else
		DTBFILE=${S_DIR}/${KERNEL_IMAGETYPE}-${KERNEL_DEVICETREE}
	fi

	IMGFILE=${S_DIR}/${KERNEL_IMAGETYPE}
	cp ${IMGFILE} ${D_DIR}
	cp ${DTBFILE} ${D_DIR}/${KERNEL_DEVICETREE}

	make_ext4_image ${PART_BOOT_LABEL} ${PART_BOOT_SIZE} ${D_DIR} ${IMAGE}
}

PART_ROOTFS_LABEL ?= "rootfs"
PART_ROOTFS_SIZE ?= ""
EXTRA_ROOTFS_DIR ?= "${BASE_WORKDIR}/extra-rootfs-support"

make_ext4_rootfsimg() {
	ROOT_FS=$1
	EXTRA_DIR=${EXTRA_ROOTFS_DIR}
	ROOT_DIR=${DEPLOY_DIR_IMAGE}/rootfs
	IMAGE=${DEPLOY_DIR_IMAGE}/rootfs.img

	if [ -z ${PART_ROOTFS_SIZE} ]; then
		echo "NOT DEFINED PART_ROOTFS_SIZE: ${PART_ROOTFS_SIZE}"
		return
	fi

	if [ -e "${ROOT_FS}.tar.gz" ]; then
		if [ -d "${ROOT_DIR}" ]; then
			rm -rf ${ROOT_DIR}
		fi
		mkdir -p ${ROOT_DIR}
		tar xvzfp ${ROOT_FS}.tar.gz -C ${ROOT_DIR};
		deploy_image=1
	elif [ -e "${ROOT_FS}.cpio.gz" ]; then
		if [ -d "${ROOT_DIR}" ]; then
			rm -rf ${ROOT_DIR}
		fi
		mkdir -p ${ROOT_DIR}
		gzip -dc ${ROOT_FS}.cpio.gz > ${DEPLOY_DIR_IMAGE}/${ROOT_FS}.cpio
		cd ${ROOT_DIR}
		cpio -idv < ${DEPLOY_DIR_IMAGE}/${ROOT_FS}.cpio;
		deploy_image=1
	elif [ -e "${ROOT_FS}.ext4" ]; then
		resize2fs ${ROOT_FS}.ext4 ${PART_ROOTFS_SIZE};
		e2fsck -y -f ${PART_ROOTFS_SIZE};
		deploy_image=0
		return;
	elif [ -d "${ROOT_FS}" ]; then
		ROOT_DIR=${ROOT_FS}
		deploy_image=0
	else
		echo "NOT FIND ROOT FS: ${ROOT_FS}"
		return
	fi

	# copy extra rootfs
	if [ -d ${EXTRA_DIR} ]; then
		cp -rp ${EXTRA_DIR}/* ${ROOT_DIR}/
	fi

	make_ext4_image ${PART_ROOTFS_LABEL} ${PART_ROOTFS_SIZE} \
		${ROOT_DIR} ${IMAGE}

}
