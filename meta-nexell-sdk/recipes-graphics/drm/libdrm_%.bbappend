DISABLE_STATIC = ""

EXTRA_OECONF += "--enable-static \
                "

FILES_${PN}-staticdev = "${libdir}/*.a"
