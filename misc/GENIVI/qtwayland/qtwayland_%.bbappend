FILESEXTRAPATHS_append := ":${THISDIR}/${PN}"

SRC_URI_append = "\
    file://0001-qtwayland_nexell_5.7.patch \
"

DEPENDS_append_koelsch = " libegl gles-user-module"
