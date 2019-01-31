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
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/etc/rc.local
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/etc/fstab

    if [ "${ARCH_TYPE}" == "arm32" ]; then
        rm -rf ${BASE_WORKDIR}/extra-rootfs-support/etc/init/ttyAMA3.conf
        rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libMali.so
        rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libEGL.so
        rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libGLESv1_CM.so
        rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libGLESv2.so
    else
        rm -rf ${BASE_WORKDIR}/extra-rootfs-support/etc/init/ttySAC3.conf
        rm -rf ${BASE_WORKDIR}/extra-rootfs-support/etc/init/failsafe.conf
    fi

    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/memtester
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/sysbench
    rm -rf ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/kselftests

    cp -rp ./boot/*                  ${BASE_WORKDIR}/extra-rootfs-support/boot/
    cp -rp ./lib/modules             ${BASE_WORKDIR}/extra-rootfs-support/lib/
    cp -a  ./usr/lib/libkms*         ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/
    cp -rp ./etc/rc.local            ${BASE_WORKDIR}/extra-rootfs-support/etc/
    cp -rp ./etc/fstab               ${BASE_WORKDIR}/extra-rootfs-support/etc/

    if [ "${ARCH_TYPE}" == "arm32" ]; then
        cp -rp ./etc/init/ttyAMA3.conf   ${BASE_WORKDIR}/extra-rootfs-support/etc/init/
        cp -rp ./usr/lib/nexell/libMali-32-X11.so    ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libMali.so
        cp -rp ./usr/lib/libEGL.so                   ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/
        cp -rp ./usr/lib/libGLESv1_CM.so             ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/
        cp -rp ./usr/lib/libGLESv2.so                ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/
    else
        cp -rp ./etc/init/ttySAC3.conf   ${BASE_WORKDIR}/extra-rootfs-support/etc/init/
        cp -rp ./etc/init/failsafe.conf  ${BASE_WORKDIR}/extra-rootfs-support/etc/init/
    fi

    cp -a ./usr/bin/start_adbd.sh     ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/

    chmod 755 ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/start_adbd.sh

    cp -a ./usr/bin/memtester      ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/
    cp -a ./usr/bin/sysbench       ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/

    # kselftests binaries
    # arm64(s5p6818) usging prebuilt binary in snapshot server, see tools/copyFilesToOutDir.sh
    if [ "${ARCH_TYPE}" == "arm32" ]; then
        cp -a ./usr/bin/kselftests       ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/
    fi
}

ROOTFS_POSTPROCESS_COMMAND +=  " postprocess_common_function;"
