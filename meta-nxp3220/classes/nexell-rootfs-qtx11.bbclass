python postprocess_rootfs_qtx11() {
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

ROOTFS_POSTPROCESS_COMMAND += " postprocess_rootfs_qtx11;"
