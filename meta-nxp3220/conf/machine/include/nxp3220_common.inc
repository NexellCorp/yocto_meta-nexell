#@TYPE: COMMON
#
# Environments for agetty (automatic login)
AGETTY_AUTOLOGIN_INITTAB_ARGUMENTS ?= "--autologin root 115200 ttyS2"

#
# Environments for disk partition images
#
PART_BOOT_LABEL ?= "boot"
PART_BOOT_SIZE ?= ""
PART_BOOT_IMAGE ?= "boot.img"

PART_ROOTFS_LABEL ?= "rootfs"
PART_ROOTFS_SIZE ?= ""
PART_ROOTFS_IMAGE ?= "rootfs.img"

PART_DATA_LABEL ?= "data"
PART_DATA_SIZE ?= ""
PART_DATA_IMAGE ?= "userdata.img"
# for fstab to auto mount at boot-time
PART_DATA_NODE ?= "/dev/mmcblk0p4"

# for misc partition image
PART_MISC_LABEL ?= "misc"
PART_MISC_SIZE ?= ""
PART_MISC_IMAGE ?= "misc.img"
# for fstab to auto mount at boot-time
PART_MISC_NODE ?= ""

# for recovery filesystem
RECOVERY_IMAGE ?= "recovery.uinitrd"

# for flash (NAND) parameter
FLASH_PAGE_SIZE ?= ""
FLASH_BLOCK_SIZE ?= ""
FLASH_DEVICE_SIZE ?= ""

# for flash ubi images
# IMAGE_CLASSES += " nexell-image-types"
# IMAGE_FSTYPES += " multiubi2"
# IMAGE_FSTYPES_DEBUGFS += " multiubi2"
# KUBIFS_ARGS_<type> options : Each partition is separated by ','
#  -r : input root filesystem for ubi image
#  -v : ubi volume/output name (<volume name>.img)
#  -i : ubi volume id
#  -l : ubi volume size
#
# Reserved ubi images type
#  boot.img    : MKUBIFS_ARGS_boot = "-r ${DEPLOY_DIR_IMAGE}/boot -v boot -i <id> -l <size>"
#  rootfs.img  : MKUBIFS_ARGS_rootfs = "-r ${IMAGE_ROOTFS} -v rootfs -i <id> -l <size>"
#  userdata.img: MKUBIFS_ARGS_data = "-r ${DEPLOY_DIR_IMAGE}/data -v userdata -i <id> -l <size>"
#  misc.img    : MKUBIFS_ARGS_misc = "-r ${DEPLOY_DIR_IMAGE}/misc -v misc -i <id> -l <size>"
#  Note>
#   If use 'MKUBIFS_ARGS_data' and want automount at boot-time for data partition,
#   you must be set 'PART_DATA_NODE = "ubiX:data"' for "/etc/fstab".
#   If use 'MKUBIFS_ARGS_misc' and want automount at boot-time for misc partition,
#   you must be set 'PART_MISC_NODE = "ubiX:misc"' for "/etc/fstab".

#
# Environments for recovery with SWUpdate
#
# Depend swupdate build config at meta-nxp3220/recipes-bbappend/swupdate/files/defconfig
#
# SWUpdate recipe:
#  DISTRO_FEATURES_append = " swupdate"
# -Set swupdate version
#  PREFERRED_VERSION_swupdate = "2019.11"
#
# General options for image:
# -Set swupdate output image name, default nxp3220-1.0.swu
#  SWU_UPDATE_SWU_IMAGE = "nxp3220-1.0.swu"
# -Set sw-description file
#  SWU_SWDESCRIPTION = "${BSP_ROOT_DIR}/tools/files/sw-description"
# -Set script files in sw-description
#  SWU_SCRIPT_FILES = "${BSP_ROOT_DIR}/tools/files/swu-script.sh"
#
# -Install versions to /etc/hwrevision, /etc/sw-versions:
#  SWU_HW_REVISION_TARGET = "nxp3220 2.0"
#  SWU_SW_VERSION_FILE = "${BSP_ROOT_DIR}/tools/files/sw-version_n.x"
#
# Signing options for image:
# -Enable signing with password
#  SWU_SIGN_ENABLE = "true"  <- sha256 with no password
#  SWU_SIGN_PASSWORD = "nxp3220" <- aes256 with password
# -Set private key file to signing swuimage
#  SWU_SIGN_PRIVATE_KEY = "${BSP_ROOT_DIR}/tools/files/swu.private.key"
# -Install public key file to /etc/swu.public.key
#  SWU_SIGN_PUBLIC_KEY = "${BSP_ROOT_DIR}/tools/files/swu.public.key"
#
# Replace services with 'recipes-bbappend/swupdate/files/swupdate, ...'
#  SWU_DAEMON_SERVICES_REPLACE = "true"
# Service options:
# swupdate options : -k <key>: public key file, -v: verbose, -n: not install -l <num>: log level
# progress options : -r: reboot, -m recovery: reboot recovery, -w: wait -p: psplash progress
#
# -Register swupdate udev rules and daemon, progress service and for swu root:
#  -udev-rules : /etc/udev/rules.d/swupdate.rules
#  -swupdate-daemon : /etc/init.d/swupdate
#  -swupdate progress service : /etc/init.d/swupdate
#  SWU_UDEV_RULES_MMC_DEVICE = "mmcblk1p[0-9]"
#  SWU_SERVICE_SWUPDATE_ROOT = "/usr/bin/swupdate -k /misc/etc/swu.public.key -i /mnt/*.swu -v -l6 -n"
#  SWU_SERVICE_PROGRESS_ROOT = "/usr/bin/swupdate-progress -r -w -m recovery"
# -Register swupdate udev rules and daemon, progress service and for recovery root:
#  -udev-rules : /etc/udev/rules.d/swupdate.rules
#  -swupdate-daemon : /etc/init.d/swupdate
#  -swupdate progress service : /etc/init.d/swupdate-progress <- /etc/rcS.d/S03swupdate-progress
#  SWU_SERVICE_SWUPDATE_RECOVERY = "/usr/bin/swupdate -k /misc/etc/swu.public.key -i /mnt/*.swu -v -l6"
#  SWU_SERVICE_PROGRESS_RECOVERY = "/usr/bin/swupdate-progress -w -r"
# -Install psplash to show update progress
#  SWU_PROGRESS_PSPLASH ?= true

# Generate swu image (SWU_UPDATE_SWU_IMAGE) with 'swu_image.sh' script and sw-description
SWU_TOOL_IMAGE_GEN = "swu_image.sh"
SWU_SUPPORT_MAKE_SWU_IMAGE ?= "true"

PREFERRED_VERSION_swupdate ?= "2019.11"
SWU_DAEMON_SERVICES_REPLACE ?= "true"
SWU_RECOVERY_ROOTFS_IMAGE ?= "nexell-recoveryimg-${MACHINE_ARCH}.cpio.gz.u-boot"

SWU_UPDATE_SWU_IMAGE ?= "nxp3220-image-2.0.swu"
SWU_SWDESCRIPTION ?= "${BSP_ROOT_DIR}/tools/files/swu-sw-description_emmc"
SWU_SCRIPT_FILES ?= "${BSP_ROOT_DIR}/tools/files/swu-script-preinstall.sh ${BSP_ROOT_DIR}/tools/files/swu-script-postinstall.sh"
SWU_HW_REVISION_TARGET ?= "nxp3220 2.0"
SWU_SW_VERSION_FILE ?= "${BSP_ROOT_DIR}/tools/files/swu-sw-versions_2.0"
SWU_SIGN_PASSWORD ?= "nxp3220"
SWU_SIGN_PRIVATE_KEY ?= "${BSP_ROOT_DIR}/tools/files/swu.private.key"
SWU_SIGN_PUBLIC_KEY ?= "${BSP_ROOT_DIR}/tools/files/swu.public.key"

SWU_SERVICE_SWUPDATE_RECOVERY ?= "/usr/bin/swupdate -k /misc/etc/swu.public.key -i /mnt/*.swu -v -l6"
SWU_SERVICE_PROGRESS_RECOVERY ?= "/usr/bin/swupdate-progress -r -w"

SWU_PROGRESS_PSPLASH ?= "false"

#
# Environments for devices
#
# for bluethooth
BSA_SERVICE_COMMAND ?= "/usr/bin/bsa_server -d /dev/ttyS0 -p /etc/bluetooth/BCM434545.hcd -all=0"
