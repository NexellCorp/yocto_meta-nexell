### Nexell - For Yocto build with using local source

EXTERNALSRC = "${BSP_ROOT_DIR}/solutions/allgo-connectivity-sdk"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"

S = "${WORKDIR}/${EXTERNALSRC}"

SRC_URI = "file://${EXTERNALSRC}"
