DESCRIPTION = "Different utilities from Android - based on the corresponding ubuntu package"
SECTION = "console/utils"
LICENSE = "Apache-2.0 & GPL-2.0 & BSD-2-Clause & BSD-3-Clause"
LIC_FILES_CHKSUM = " \
  file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10 \
  file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6 \
  file://${COMMON_LICENSE_DIR}/BSD-2-Clause;md5=8bef8e6712b1be5aa76af1ebde9d6378 \
  file://${COMMON_LICENSE_DIR}/BSD-3-Clause;md5=550794465ba0ec5312d6919e203a55f9 \
  file://${WORKDIR}/debian/copyright;md5=141efd1050596168ca05ced04e4f498b \
"

DEPENDS = "zlib openssl"

BBCLASSEXTEND += "native"

SRC_URI = " \
    file://android-tools.tar.gz \
    file://android-tools-debian.tar.gz \
    file://android-tools-adbd.service \
    file://android-tools-adbd-daudio_ref.service \
    file://start_adbd-32.sh \
    file://start_adbd-64.sh \
    file://start_adbd-daudio_ref.sh \
    file://stop_adbd.sh \
    "

S = "${WORKDIR}/android-tools"

inherit systemd

SYSTEMD_SERVICE_${PN} = "android-tools-adbd.service"

ADBD_SERVICE = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-daudio-ref', 'android-tools-adbd-daudio_ref.service', 'android-tools-adbd.service', d)}"
ADBD_START_SCRIPT = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-daudio-ref', 'start_adbd-daudio_ref.sh', 'start_adbd-${ARCH_TYPE_NUM}.sh', d)}"

do_compile() {
    unset CFLAGS
    unset CPPFLAGS

    sed -i "s%^CPPFLAGS+= -I/usr/include%# we don't want to include headers from host CPPFLAGS+= -I/usr/include%g" ${WORKDIR}/debian/makefiles/ext4_utils.mk
    # sed -i "s/LIBS+= -lz -lselinux/LIBS+= -lz/g" ${WORKDIR}/debian/makefiles/ext4_utils.mk

    oe_runmake -f ${WORKDIR}/debian/makefiles/adbd.mk -C ${S}/core/adbd clean
    oe_runmake -f ${WORKDIR}/debian/makefiles/adbd.mk -C ${S}/core/adbd
    # oe_runmake -f ${WORKDIR}/debian/makefiles/ext4_utils.mk -C ${S}/extras/ext4_utils clean
    # oe_runmake -f ${WORKDIR}/debian/makefiles/ext4_utils.mk -C ${S}/extras/ext4_utils
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/core/adbd/adbd ${D}${bindir}

    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/${ADBD_SERVICE} ${D}${systemd_unitdir}/system/android-tools-adbd.service

    install -m 0755 ${WORKDIR}/${ADBD_START_SCRIPT} ${D}${bindir}/start_adbd.sh
    install -m 0755 ${WORKDIR}/stop_adbd.sh ${D}${bindir}/
}

FILES_${PN} = "${bindir} ${base_sbindir}"
