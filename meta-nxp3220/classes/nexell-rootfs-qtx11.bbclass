postprocess_qtx11_func1() {
    cd ${IMAGE_ROOTFS}
    # X11 touch pointer default calibrate running prevent
    # If tslib use, have to remove 30xinput_calibrate.sh
    # rm etc/X11/Xsession.d/30xinput_calibrate.sh

    # If xf86_tslib use, have to remove xinput_calibrator.desktop
    # rm etc/xdg/autostart/xinput_calibrator.desktop
}

python postprocess_qtx11_func2() {
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

ROOTFS_POSTPROCESS_COMMAND += " postprocess_qtx11_func1; postprocess_qtx11_func2;"
