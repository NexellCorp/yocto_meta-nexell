include nexell-common.local.inc

IMAGE_CLASSES = " s5p4418-post-process-qt s5p4418-post-process"

NEXELL_32_COMMON_INSTALL = " \
    rtl-8188eus-32 \
    "

IMAGE_INSTALL_append = " \
    ${NEXELL_32_COMMON_INSTALL} \
    "
