# to make images

DEPENDS += "${@bb.utils.contains('IMAGE_FSTYPES', 'ext4', 'android-tools-native', '', d)}"
DEPENDS += "${@bb.utils.contains('IMAGE_FSTYPES', 'multiubi2', 'mtd-utils-native', '', d)}"

make_ext4_image() {
	local label=$1 size=$2 dir=$3
	local output=$4

	echo "NOTE: MAKE EXT4: $label ($size) $dir -> $output"

	# change to sparse image "-s" optioin
	make_ext4fs -L ${label} -s -b 4k -l ${size} ${output} ${dir}
}

PART_BOOT_LABEL ?= "boot"
PART_BOOT_SIZE ?= ""
PART_BOOT_IMAGE ?= "boot.img"
UBOOT_LOGO_BMP ?= ""

make_boot_image() {
	local deplaydir=$1
	local bootdir=${deplaydir}/${PART_BOOT_LABEL}
	local bootimg=${deplaydir}/${PART_BOOT_IMAGE}
	local kernelimg=${deplaydir}/${KERNEL_IMAGETYPE}
	local logobmp=${UBOOT_LOGO_BMP}
	local dtbfile

	mkdir -p ${bootdir}

	if [ -e ${deplaydir}/${KERNEL_DEVICETREE} ]; then
		dtbfile=${deplaydir}/${KERNEL_DEVICETREE}
	else
		dtbfile=${deplaydir}/${KERNEL_IMAGETYPE}-${KERNEL_DEVICETREE}
	fi

	cp ${kernelimg} ${bootdir}
	cp ${dtbfile} ${bootdir}/${KERNEL_DEVICETREE}

	if [ -f "${UBOOT_LOGO_BMP}" ]; then
		install -m 0644 ${logobmp} ${bootdir};
	fi

	if ${@bb.utils.contains('IMAGE_FSTYPES','ext4','true','false',d)}; then
		if [ -z ${PART_BOOT_SIZE} ]; then
			echo "WARNING: NOT DEFINED 'PART_BOOT_SIZE'"
			return
		fi

		make_ext4_image ${PART_BOOT_LABEL} ${PART_BOOT_SIZE} ${bootdir} ${bootimg}
	fi

	if ${@bb.utils.contains('IMAGE_FSTYPES','multiubi2','true','false',d)}; then
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

make_rootfs_image() {
	local root=$1
	local rootimg=${DEPLOY_DIR_IMAGE}/$2
	local rootlen=$3

	if ${@bb.utils.contains('IMAGE_FSTYPES','ext4','true','false',d)}; then
		if [ -z ${rootlen} ]; then
			echo "WARNING: NOT DEFINED 'SIZE'"
			return
		fi

		if [ ! -e "${root}.ext4" ]; then
			echo "WARNING: NOT FOUND EXT4 ROOT FS: ${root}"
			return
		fi

		local fsname=$(readlink -f ${root}.ext4)
		local fssize=$(wc -c < ${fsname})
		echo "DEBUG: Resize ROOTS minimun size : ${fssize}:${fsname}"

		resize2fs ${root}.ext4 ${rootlen};
		e2fsck -y -f ${root}.ext4;

		# change to sparse image
		ext2simg ${root}.ext4 ${rootimg}
	fi
}

PART_DATA_LABEL ?= "data"
PART_DATA_SIZE ?= ""
PART_DATA_IMAGE ?= "userdata.img"

make_data_image() {
	local deplaydir=$1
	local datadir=${deplaydir}/${PART_DATA_LABEL}
	local dataimg=${deplaydir}/${PART_DATA_IMAGE}

	mkdir -p ${datadir}

	if ${@bb.utils.contains('IMAGE_FSTYPES','ext4','true','false',d)}; then
		if [ -z ${PART_DATA_SIZE} ]; then
			echo "WARNING: NOT DEFINED 'PART_DATA_SIZE'"
			return
		fi

		make_ext4_image ${PART_DATA_LABEL} ${PART_DATA_SIZE} ${datadir} ${dataimg}
	fi

	if ${@bb.utils.contains('IMAGE_FSTYPES','multiubi2','true','false',d)}; then
		if [ -z ${FLASH_PAGE_SIZE} ] || [ -z ${FLASH_BLOCK_SIZE} ] ||
		   [ -z ${FLASH_DEVICE_SIZE} ]; then
			bbfatal "Not defined 'FLASH_PAGE_SIZE or FLASH_BLOCK_SIZE or FLASH_DEVICE_SIZE'"
		fi

		local images="$(echo ${MKUBIFS_ARGS_data} | awk -F',' '{ print NF }')"
		local i=1

		while [ $i -le $images ];
		do
		        args="$(echo $(echo ${MKUBIFS_ARGS_data}| cut -d',' -f $i))"
			args="$args -p ${FLASH_PAGE_SIZE} -b ${FLASH_BLOCK_SIZE} -c ${FLASH_DEVICE_SIZE}"

			make_ubi_image ${args}

		        i=`expr $i + 1`
		done;
	fi
}

PART_MISC_LABEL ?= "misc"
PART_MISC_SIZE ?= ""
PART_MISC_IMAGE ?= "misc.img"

make_misc_image() {
	local deplaydir=$1
	local miscimg=${deplaydir}/${PART_MISC_IMAGE}
	local miscdir=${deplaydir}/${PART_MISC_LABEL}
	local etcdir=${miscdir}/etc

	# remove old misc directory
	if [ -d ${miscdir} ]; then
		rm -rf ${miscdir}
	fi

	mkdir -p ${miscdir}
	mkdir -p ${etcdir}

        # install swupdate signing key file
        #
        if [ ! -z ${SWU_SIGN_PUBLIC_KEY} ] && [ -f ${SWU_SIGN_PUBLIC_KEY} ]; then
                install -m 644 ${SWU_SIGN_PUBLIC_KEY} ${etcdir}/swu.public.key
        fi

        # install swupdate version
        #
        if [ ! -z "${SWU_HW_REVISION_TARGET}" ]; then
                echo ${SWU_HW_REVISION_TARGET} > ${etcdir}/hwrevision
        fi

        if [ ! -z ${SWU_SW_VERSION_FILE} ] && [ -f ${SWU_SW_VERSION_FILE} ]; then
                install -m 644 ${SWU_SW_VERSION_FILE} ${etcdir}/sw-versions
        fi

	# generate misc.img
	#
	if ${@bb.utils.contains('IMAGE_FSTYPES','ext4','true','false',d)}; then
		if [ -z ${PART_MISC_SIZE} ]; then
			echo "WARNING: NOT DEFINED 'PART_MISC_SIZE'"
			return
		fi

		make_ext4_image ${PART_MISC_LABEL} ${PART_MISC_SIZE} ${miscdir} ${miscimg}
	fi

	if ${@bb.utils.contains('IMAGE_FSTYPES','multiubi2','true','false',d)}; then
		if [ -z ${FLASH_PAGE_SIZE} ] || [ -z ${FLASH_BLOCK_SIZE} ] ||
		   [ -z ${FLASH_DEVICE_SIZE} ]; then
			bbfatal "Not defined 'FLASH_PAGE_SIZE or FLASH_BLOCK_SIZE or FLASH_DEVICE_SIZE'"
		fi

		local images="$(echo ${MKUBIFS_ARGS_misc} | awk -F',' '{ print NF }')"
		local i=1

		while [ $i -le $images ];
		do
		        args="$(echo $(echo ${MKUBIFS_ARGS_misc}| cut -d',' -f $i))"
			args="$args -p ${FLASH_PAGE_SIZE} -b ${FLASH_BLOCK_SIZE} -c ${FLASH_DEVICE_SIZE}"

			make_ubi_image ${args}

		        i=`expr $i + 1`
		done;
	fi
}

EXTRA_ROOTFS_DIR ?= "${BASE_WORKDIR}/extra-rootfs-support"

make_rootfs_extra() {
	if [ ! -d "${EXTRA_ROOTFS_DIR}" ]; then
		echo "WARNING: not found ${EXTRA_ROOTFS_DIR}"
		return
	fi

	cp -dr ${EXTRA_ROOTFS_DIR}/* ${IMAGE_ROOTFS}
}

convert_hn_to_byte() {
	local val=$1
	local ret=$2 # store calculated byte
	local delmi="" mulitple=0

	case "$val" in
        *K* ) delmi='K'; mulitple=1024;;
        *k* ) delmi='k'; mulitple=1024;;
        *M* ) delmi='M'; mulitple=1048576;;
        *m* ) delmi='m'; mulitple=1048576;;
        *G* ) delmi='G'; mulitple=1073741824;;
        *g* ) delmi='g'; mulitple=1073741824;;
        -- ) ;;
        esac

        if [ ! -z $delmi ]; then
                val=$(echo $val| cut -d$delmi -f 1)
		val=`expr $val \* $mulitple`
		eval "$ret=\"${val}\""
        fi
}

create_ubi_ini_file() {
	local ini_file=$1 image=$2 vname=$3 vid=$4 vsize=$5

        echo \[ubifs\] > $ini_file
        echo mode=ubi >> $ini_file
        echo image=$image >> $ini_file
        echo vol_id=$vid >> $ini_file
        echo vol_size=$vsize >> $ini_file
        echo vol_type=dynamic >> $ini_file
        echo vol_name=$vname >> $ini_file
        echo vol_flags=autoresize >> $ini_file
}

# -r : root image
# -v : volume name
# -l : volume size
# -i : volume id
# -p : page size
# -s : sub page size
# -b : block size
# -c : flash size
make_ubi_image() {
	ARGS=$(getopt -o p:s:b:c:r:v:l:i: -- "$@");
	eval set -- "$ARGS";

	local volume_id=0

	while true; do
		case "$1" in
		-p ) page_size=$2; shift 2;;
		-s ) sub_page_size=$2; shift 2;;
		-b ) block_size=$2; shift 2;;
		-c ) flash_size=$2; shift 2;;
		-r ) root=$2; shift 2;;
		-v ) volume_name=$2; shift 2;;
		-l ) volume_size=$2; shift 2;;
		-i ) volume_id=$2; shift 2;;
		-- ) break ;;
		esac
	done

	if [ -z $page_size ] || [ -z $block_size ] || [ -z $flash_size ]; then
		bbfatal "Not defined ubi page size '-p' or block size '-b' or flash size '-c'"
	fi

	if [ -z $sub_page_size ] ||  [ $sub_page_size -eq 0 ]; then
		sub_page_size=$page_size
	fi

	if [ -z $root ] || [ -z $volume_name ] || [ -z $volume_size ]; then
		bbfatal "Not defined ubi root file '-r' volume name '-o' or ubi volume size '-l'"
	fi

	convert_hn_to_byte $block_size block_size
	convert_hn_to_byte $flash_size flash_size
	convert_hn_to_byte $volume_size volume_size

	#
	# Calcurate UBI varialbe
	# Refer to http://processors.wiki.ti.com/index.php/UBIFS_Support
	#
	local LEB=`expr $block_size - \( 2 \* $page_size \)`
	local PEB=$block_size
	local BLC=`expr $flash_size / $block_size`
	local RPB=`expr \( 20 \* $BLC \) / 1024`
	local RPC=`expr $PEB - $LEB`
	local TPB=`expr $volume_size / $PEB`
	local OVH=`expr \( \( $RPB + 4 \) \* $PEB \) + \( $RPC \* \( $TPB - $RPB - 4 \) \)`
	local OVB=`expr $OVH / $PEB`

	local avail_size=`expr $volume_size - $OVH`
	local max_block_count=`expr $avail_size / $LEB`

	local deploy_dir=${IMGDEPLOYDIR}
	if [ -z $img_deplay_dir ]; then
		deploy_dir=${DEPLOY_DIR_IMAGE}
	fi

	local ubi_fs=$deploy_dir/$volume_name.ubifs
	local ubi_image=$deploy_dir/$volume_name.img
	local ubi_ini=$deploy_dir/ubinize-$volume_name.cfg

	echo "DEBUG: ROOT dir = $root"
	echo "DEBUG: UBI fs = $ubi_fs"
	echo "DEBUG: UBI image = $ubi_image"
	echo "DEBUG: UBI ini = $ubi_ini"
	echo "DEBUG: UBI Volume name = $volume_name"
	echo "DEBUG: UBI Volume id = $volume_id"
	echo "DEBUG: UBI Volume size = `expr $avail_size / 1024 / 1024`MiB (`expr $volume_size / 1024 / 1024`MiB)"
	echo "DEBUG: UBI Logical Erase Block size = `expr $LEB / 1024`KiB"
	echo "DEBUG: UBI Maximum Lobical Erase block counts = $max_block_count"
	echo "DEBUG: UBI Overhead = $OVB ($TPB)"
	echo "DEBUG: UBI Reserved size = `expr $OVH / 1024 / 1024`MiB"
	echo "DEBUG: Flash Page size = $page_size"
	echo "DEBUG: Flash Sub page size = $sub_page_size"
	echo "DEBUG: Flash Block size = `expr $block_size / 1024`KiB"
	echo "DEBUG: Flash size = `expr $flash_size / 1024 / 1024`MiB"

	mkdir -p $root

	create_ubi_ini_file $ubi_ini $ubi_fs \
		$volume_name $volume_id `expr $avail_size / 1024 / 1024`MiB

	mkfs.ubifs -r $root -o $ubi_fs -m $page_size -e $LEB -c $max_block_count -F

	ubinize -o $ubi_image -m $page_size -p $block_size -s $sub_page_size $ubi_ini
}
