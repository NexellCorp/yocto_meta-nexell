### Nexell - For Yocto build with using local source

EXTERNALSRC = "${BSP_ROOT_DIR}/library/gst-plugins-video-sink"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"

S = "${WORKDIR}/${EXTERNALSRC}"

SRC_URI = "file://${EXTERNALSRC}"
