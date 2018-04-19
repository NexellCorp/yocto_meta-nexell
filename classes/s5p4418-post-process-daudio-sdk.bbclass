DESCRIPTION = "Nexell daudio_ref SDK"
LICENSE = "CLOSED"

postprocess_install_daudio_sdk_function() {
    install -d ${IMAGE_ROOTFS}/etc/bluetooth
    install -d ${IMAGE_ROOTFS}/usr/bin
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxAVIn/lib
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxAudioPlayer/lib
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxBTAudioR
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxBTPhoneR
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxBTSettingsR
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxQuickRearCam/lib
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxVideoPlayer
    install -d ${IMAGE_ROOTFS}/usr/lib
    install -d ${IMAGE_ROOTFS}/podo/apps/pdwindowcompositor
    install -d ${IMAGE_ROOTFS}/lib/systemd/system

    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/etc/bluetooth/BCM20710A1_001.002.014.0103.0000.hcd ${IMAGE_ROOTFS}/etc/bluetooth/

    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxService.sh ${IMAGE_ROOTFS}/usr/bin/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxBTServiceConsole ${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxBTServiceR ${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxCapture ${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxDAudioManager ${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxLauncher ${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/bsa_server ${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/key_input ${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxCommandSender ${IMAGE_ROOTFS}/usr/bin/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxAVIn/NxAVIn ${IMAGE_ROOTFS}/nexell/daudio/NxAVIn/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxAVIn/NxAVIn.desktop ${IMAGE_ROOTFS}/nexell/daudio/NxAVIn/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxAVIn/icon_auxin.png ${IMAGE_ROOTFS}/nexell/daudio/NxAVIn/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxAVIn/lib/* ${IMAGE_ROOTFS}/nexell/daudio/NxAVIn/lib/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxAudioPlayer/NxAudioPlayer ${IMAGE_ROOTFS}/nexell/daudio/NxAudioPlayer/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxAudioPlayer/NxAudioPlayer.desktop ${IMAGE_ROOTFS}/nexell/daudio/NxAudioPlayer/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxAudioPlayer/icon_music.png ${IMAGE_ROOTFS}/nexell/daudio/NxAudioPlayer/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxAudioPlayer/lib/* ${IMAGE_ROOTFS}/nexell/daudio/NxAudioPlayer/lib/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTAudioR/NxBTAudioR ${IMAGE_ROOTFS}/nexell/daudio/NxBTAudioR/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTAudioR/NxBTAudioR.desktop ${IMAGE_ROOTFS}/nexell/daudio/NxBTAudioR/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTAudioR/icon_btaudior.png ${IMAGE_ROOTFS}/nexell/daudio/NxBTAudioR/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTPhoneR/NxBTPhoneR ${IMAGE_ROOTFS}/nexell/daudio/NxBTPhoneR/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTPhoneR/NxBTPhoneR.desktop ${IMAGE_ROOTFS}/nexell/daudio/NxBTPhoneR/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTPhoneR/icon_btphoner.png ${IMAGE_ROOTFS}/nexell/daudio/NxBTPhoneR/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTSettingsR/NxBTSettingsR ${IMAGE_ROOTFS}/nexell/daudio/NxBTSettingsR/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTSettingsR/NxBTSettingsR.desktop ${IMAGE_ROOTFS}/nexell/daudio/NxBTSettingsR/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTSettingsR/icon_btsettingsr.png ${IMAGE_ROOTFS}/nexell/daudio/NxBTSettingsR/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxQuickRearCam/NxQuickRearCam ${IMAGE_ROOTFS}/nexell/daudio/NxQuickRearCam/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxQuickRearCam/NxQuickRearCam.desktop ${IMAGE_ROOTFS}/nexell/daudio/NxQuickRearCam/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxQuickRearCam/rearcam.png ${IMAGE_ROOTFS}/nexell/daudio/NxQuickRearCam/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxQuickRearCam/lib/* ${IMAGE_ROOTFS}/nexell/daudio/NxQuickRearCam/lib/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxVideoPlayer/NxVideoPlayer ${IMAGE_ROOTFS}/nexell/daudio/NxVideoPlayer/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxVideoPlayer/NxVideoPlayer.desktop ${IMAGE_ROOTFS}/nexell/daudio/NxVideoPlayer/
    install -m 0755 --owner=root --group=root ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxVideoPlayer/icon_video.png ${IMAGE_ROOTFS}/nexell/daudio/NxVideoPlayer/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/lib/* ${IMAGE_ROOTFS}/usr/lib/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/podo/podo ${IMAGE_ROOTFS}/podo/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/podo/apps/pdwindowcompositor/pdwindowcompositor ${IMAGE_ROOTFS}/podo/apps/pdwindowcompositor/

    install -m 0644 ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/lib/systemd/system/android-tools-adbd.service ${IMAGE_ROOTFS}/lib/systemd/system/
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_daudio_sdk_function;"
