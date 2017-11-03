FILESEXTRAPATHS_prepend := "${THISDIR}/busybox:"

SRC_URI_append = " \
    file://__get_devname.c \
    "

do_postapply() {
    mv ${WORKDIR}/__get_devname.c ${S}/util-linux/volume_id/get_devname.c
}
addtask postapply before do_compile after do_configure
