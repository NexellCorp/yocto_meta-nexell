require mali-nexell.inc

TYPE = "mali-nexell-2016"

SRC_URI += " \
    file://bin \
    file://include \
    file://lib \
    file://module \
    file://egl.pc \
    file://gles2.pc \
    file://glesv2.pc \
    file://glesv1_cm.pc \
    file://vg.pc \
    file://directfbrc \
    "
LICENSE = "CLOSED"
#LIC_FILES_CHKSUM = "file://COPYING;md5=3dc5bc15b0c8fdcb598b33ccdbd32e2c"
