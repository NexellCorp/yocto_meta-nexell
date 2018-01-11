inherit linux-nexell-base

DEFAULT_QT_PLATFORM ?= "${@bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-x11', 'X11', '', d)}"

postprocess_qtx11_function() {
    cd ${IMAGE_ROOTFS}

    #X11 touch pointer default calibrate running prevent
    # If tslib use, have to remove 30xinput_calibrate.sh
    #rm etc/X11/Xsession.d/30xinput_calibrate.sh
    # If xf86_tslib use, have to remove xinput_calibrator.desktop
    rm etc/xdg/autostart/xinput_calibrator.desktop

    #adbd do not start on boot time, If you need use adbd, then you can  run /usr/bin/start_adbd.sh
    cd ${IMAGE_ROOTFS}/lib/systemd/system
    sed -i 's/ExecStart=\/usr\/bin\/adbd/\#ExecStart=\/usr\/bin\/adbd/g' android-tools-adbd.service

    cd ${IMAGE_ROOTFS}/usr/lib
    touch libMali_for_${DEFAULT_QT_PLATFORM}
    ln -sf ./nexell/libMali-${ARCH_TYPE_NUM}-${DEFAULT_QT_PLATFORM}.so libMali.so

    #automount iocharset modified
    cd ${IMAGE_ROOTFS}
    cp ${BASE_WORKDIR}/use-post-process/mount.sh etc/udev/scripts/
}

python postprocess_qtx11_function2() {
    import os

    os.chdir(d.getVar("IMAGE_ROOTFS", d, True))

    with open("usr/share/X11/xorg.conf.d/40-libinput.conf", 'r+') as f:
         contents = f.readlines()
         for index, line in enumerate(contents):
             if "Identifier" in line and "touchscreen" in line :
                contents.insert(index + 1, '\tOption "TransformationMatrix" "1 0 0 0 -1 1 0 0 1"\n')
                break

         f.seek(0)
         f.writelines(contents)
}

ROOTFS_POSTPROCESS_COMMAND += " postprocess_qtx11_function; postprocess_qtx11_function2;"
