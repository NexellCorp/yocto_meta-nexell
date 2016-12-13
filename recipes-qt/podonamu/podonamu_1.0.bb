SUMMARY = "Qt5 FRAMEWORK, PODO SOFTWARE INC."
HOMEPAGE = "http://www.podovat.com"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "qtbase qtquickcontrols2 qtmultimedia qtwebkit qtwayland systemd"

PR = "r1"

SRCREV = "${AUTOREV}"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/podonamu;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

require recipes-qt/qt5/qt5.inc
do_install() {
    install -d ${D}/podo
    cp -a ${B}/apps/ ${D}/podo
    cp -a ${B}/core/podo ${D}/podo

    rm ${D}/podo/apps/*/*.cpp
    rm ${D}/podo/apps/*/*.o
    rm ${D}/podo/apps/*/*.h
    rm ${D}/podo/apps/*/Makefile
    cp -a ${S}/apps/pdhelloweb/ ${D}/podo/apps/
    cp -a ${S}/*.sh ${D}/podo
    rm ${D}/podo/install.sh
    APPS="pdhellocpp pdhelloqml pdimageview pdsetting textures pdapreceiver pdapsender"
    for i in $APPS
    do
          echo "Copying $i..."
    cp -a -r ${S}/apps/$i/pkg/* ${D}/podo/apps/$i
    done

    chmod -R 775 ${D}/podo
    install -d ${D}${systemd_unitdir}/system/ ${D}${systemd_unitdir}/system/multi-user.target.wants/
    install -Dm0644 ${S}/podonamu.service ${D}${systemd_unitdir}/system/
    ln -sf ${D}${systemd_unitdir}/system/podonamu.service ${D}${systemd_unitdir}/system/multi-user.target.wants/podonamu.service
}

PARALLEL_MAKE="-j 1"

FILES_${PN} = "podo ${systemd_system_unitdir}"

INSANE_SKIP_${PN} = "host-user-contaminated debug-files"
RDEPENDS_${PN} = "qtbase-plugins"

SYSTEMD_SERVICE_${PN} = "podonamu.service"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_AUTO_ENABLE_${PN} = "enable"
