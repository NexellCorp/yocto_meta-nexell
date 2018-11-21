FILESEXTRAPATHS_prepend := "${THISDIR}/patch:"

SRC_URI_append = " \
    file://cinematicexperience-killall-bootanim-before-show.patch \
    "
