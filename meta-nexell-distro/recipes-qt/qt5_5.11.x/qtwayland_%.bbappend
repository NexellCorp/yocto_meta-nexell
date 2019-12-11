FILESEXTRAPATHS_prepend := "${THISDIR}/qtwayland_patch:"

SRC_URI_append = " \
    file://always_true_isExposed.patch \
"

