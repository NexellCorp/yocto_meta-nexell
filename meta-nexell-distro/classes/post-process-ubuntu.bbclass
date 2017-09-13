postprocess_common_function() {
    cd ${BASE_WORKDIR}/extra-rootfs-support
    mkdir -p boot
    mkdir -p lib
    mkdir -p usr/lib
    mkdir -p etc/init

    cd ${IMAGE_ROOTFS}

    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/boot/*
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/lib/modules
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libkms*
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/etc/init/failsafe.conf
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/etc/init/ttyAMA3.conf
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/etc/rc.local
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/etc/fstab

    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libMali.so
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libEGL.so
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libGLESv1_CM.so
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libGLESv2.so

    cp -rp ./boot/*                  ${BASE_WORKDIR}/extra-rootfs-support/boot/
    cp -rp ./lib/modules             ${BASE_WORKDIR}/extra-rootfs-support/lib/
    cp -a  ./usr/lib/libkms*         ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/
    cp -rp ./etc/init/failsafe.conf  ${BASE_WORKDIR}/extra-rootfs-support/etc/init/
    cp -rp ./etc/init/ttyAMA3.conf   ${BASE_WORKDIR}/extra-rootfs-support/etc/init/
    cp -rp ./etc/rc.local            ${BASE_WORKDIR}/extra-rootfs-support/etc/
    cp -rp ./etc/fstab               ${BASE_WORKDIR}/extra-rootfs-support/etc/

    cp -rp ./usr/lib/libMali.so      ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/
    cp -rp ./usr/lib/libEGL.so       ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/
    cp -rp ./usr/lib/libGLESv1_CM.so ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/
    cp -rp ./usr/lib/libGLESv2.so    ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/

    cp -a ./usr/bin/start_adb.sh     ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/
}

ROOTFS_POSTPROCESS_COMMAND +=  " postprocess_common_function;"
