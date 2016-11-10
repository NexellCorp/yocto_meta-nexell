include nexell-common.local.inc

IMAGE_CLASSES = " s5p6818-post-process-qt s5p6818-post-process"

NEXELL_64_COMMON_INSTALL = " \
    rtl-8188eus-64 \
    "

IMAGE_INSTALL_append = " \
    ${NEXELL_64_COMMON_INSTALL} \
    "
