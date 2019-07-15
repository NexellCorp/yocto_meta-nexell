FILESEXTRAPATHS_prepend := "${THISDIR}/patch:"

SRC_URI_append = " \
    file://nexell-add-v4l2-special-macro.patch \
"
