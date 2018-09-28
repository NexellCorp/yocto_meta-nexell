inherit linux-nexell-base

DEFAULT_SDL_PLATFORM ?= "${@bb.utils.contains('DISTRO_FEATURES', 'sdl-default-platform-eglfs', 'FB', \
                           bb.utils.contains('DISTRO_FEATURES', 'sdl-default-platform-wayland', 'WAYLAND', '', d), d)}"

postprocess_sdl_function() {
    cd ${IMAGE_ROOTFS}

    #adbd do not start on boot time, If you need use adbd, then you can  run /usr/bin/start_adbd.sh
    cd ${IMAGE_ROOTFS}/lib/systemd/system
    sed -i 's/ExecStart=\/usr\/bin\/adbd/\#ExecStart=\/usr\/bin\/adbd/g' android-tools-adbd.service

    cd ${IMAGE_ROOTFS}/usr/lib
    touch libMali_for_${DEFAULT_SDL_PLATFORM}
    if [ "${ARCH_TYPE_NUM}" -eq "64" ]; then
        # 64bit FB library does not support, TBD
        ln -sf ./nexell/libMali-${ARCH_TYPE_NUM}-WAYLAND libMali.so
    else
        ln -sf ./nexell/libMali-${ARCH_TYPE_NUM}-${DEFAULT_SDL_PLATFORM}.so libMali.so
    fi

    #automount iocharset modified
    cd ${IMAGE_ROOTFS}
    cp ${BASE_WORKDIR}/use-post-process/mount.sh etc/udev/scripts/
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_sdl_function;"
