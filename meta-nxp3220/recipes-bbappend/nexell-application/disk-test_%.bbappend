### Nexell - For Yocto build with using local source

EXTERNALSRC = "${BSP_ROOT_DIR}/apps/disk_test"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"

S = "${WORKDIR}/${EXTERNALSRC}"

SRC_URI = "file://${EXTERNALSRC}"