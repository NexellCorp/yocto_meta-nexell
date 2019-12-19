DESCRIPTION = "Sample application for AMD RTC driver"
SECTION = "applications"
LICENSE = "BSD"
DEPENDS = "readline"
LIC_FILES_CHKSUM = "file://rtc-test.c;md5=2ad20cafbc5aa015ebefa0c6ca744bae"

SRC_URI = "file://rtc-test.c"

S = "${WORKDIR}"

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile() {
	${CC} rtc-test.c -o rtc-test -lreadline
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 rtc-test ${D}${bindir}
}
