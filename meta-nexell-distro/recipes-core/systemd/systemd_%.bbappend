FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
    file://local.rules \
    file://usb1-rules.sh \
    file://usb2-rules.sh \
    file://10-eth.network \
    file://30-wlan.network \
    file://60-usb.network \
"

PACKAGECONFIG = " \
    ${@bb.utils.filter('DISTRO_FEATURES', 'efi ldconfig pam selinux usrmerge', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wifi', 'rfkill', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'xkbcommon', '', d)} \
    backlight \
    binfmt \
    firstboot \
    hibernate \
    hostnamed \
    ima \
    localed \
    logind \
    machined \
    myhostname \
    networkd \
    nss \
    polkit \
    quotacheck \
    randomseed \
    resolved \
    smack \
    sysusers \
    timedated \
    utmp \
    vconsole \
    xz \
"

do_install_append() {
    install -d ${D}${sysconfdir}/udev/rules.d/
    install -m 0644 ${WORKDIR}/local.rules ${D}${sysconfdir}/udev/rules.d/

    install -d ${D}${sysconfdir}/udev/scripts/ 
    install -m 0755 ${WORKDIR}/usb1-rules.sh ${D}${sysconfdir}/udev/scripts/
    install -m 0755 ${WORKDIR}/usb2-rules.sh ${D}${sysconfdir}/udev/scripts/

    install -d ${D}${sysconfdir}/systemd/network/
    install -m 0644 ${WORKDIR}/10-eth.network ${D}${sysconfdir}/systemd/network/
    install -m 0644 ${WORKDIR}/30-wlan.network ${D}${sysconfdir}/systemd/network/
    install -m 0644 ${WORKDIR}/60-usb.network ${D}${sysconfdir}/systemd/network/

    # Allow automount from udev
    install -m 0644 ${D}${systemd_system_unitdir}/systemd-udevd.service ${D}${sysconfdir}/systemd/system/
    sed -i 's/MountFlags=slave/MountFlags=shared/g' ${D}${sysconfdir}/systemd/system/systemd-udevd.service
}