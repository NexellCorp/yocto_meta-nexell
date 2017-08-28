SUMMARY = "Qt bootlauncher"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "qtbase qtquickcontrols2 qtmultimedia qtwayland "

SRC_URI = "file://launcher-cap.conf \
           file://launcher-press.conf \
           file://Makefile \
           file://qtbootlauncher.c \
"

S = "${WORKDIR}"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="${CC}" LDFLAGS+="--sysroot=${STAGING_DIR_HOST}"
}

do_install() {
    install -d ${D}/lib/systemd/system-generators
    install -d ${D}/etc/qboot

    cp ${S}/qtbootlauncher ${D}/lib/systemd/system-generators/

    if [ "${NEXELL_TOUCH_CLASS}" = "CAPACITIVE" ]; then
        cp ${S}/launcher-cap.conf ${D}/etc/qboot/launcher.conf
    else
        cp ${S}/launcher-press.conf ${D}/etc/qboot/launcher.conf
    fi
}

FILES_${PN} = "/lib/systemd/system-generators etc/qboot"
INSANE_SKIP_${PN} = "installed-vs-shipped already-stripped"