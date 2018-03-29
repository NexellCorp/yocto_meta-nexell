DESCRIPTION = "Nexell daudio_ref SDK for rootfs"
LICENSE = "CLOSED"

postprocess_install_daudio_sdk_function() {
    install -d ${IMAGE_ROOTFS}/etc/bluetooth
    install -d ${IMAGE_ROOTFS}/usr/bin
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxAVIn/lib
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxAudioPlayer/lib
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxBTAudioR
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxBTPhoneR
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxBTSettingsR
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxDAudioManagerTest
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxQuickRearCam/lib
    install -d ${IMAGE_ROOTFS}/nexell/daudio/NxVideoPlayer
    install -d ${IMAGE_ROOTFS}/usr/lib
    install -d ${IMAGE_ROOTFS}/podo/apps/pdwindowcompositor

    cp -aR ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/etc/bluetooth/BCM20710A1_001.002.014.0103.0000.hcd	${IMAGE_ROOTFS}/etc/bluetooth/
	chown -R root.root ${IMAGE_ROOTFS}/etc/bluetooth/BCM20710A1_001.002.014.0103.0000.hcd
    cp -aR ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxService.sh								${IMAGE_ROOTFS}/usr/bin/
	chown -R root.root ${IMAGE_ROOTFS}/usr/bin/NxService.sh
    cp -aR ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/*										${IMAGE_ROOTFS}/nexell/daudio/
	chown -R root.root ${IMAGE_ROOTFS}/nexell/daudio

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/lib/*					${IMAGE_ROOTFS}/usr/lib/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxBTServiceConsole	${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxBTServiceR			${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxCapture			${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxDAudioManager		${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/NxLauncher			${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/bsa_server			${IMAGE_ROOTFS}/usr/bin/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/key_input			${IMAGE_ROOTFS}/usr/bin/

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxAVIn/NxAVIn								${IMAGE_ROOTFS}/nexell/daudio/NxAVIn
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxAudioPlayer/NxAudioPlayer				${IMAGE_ROOTFS}/nexell/daudio/NxAudioPlayer
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTAudioR/NxBTAudioR						${IMAGE_ROOTFS}/nexell/daudio/NxBTAudioR
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTPhoneR/NxBTPhoneR						${IMAGE_ROOTFS}/nexell/daudio/NxBTPhoneR
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxBTSettingsR/NxBTSettingsR				${IMAGE_ROOTFS}/nexell/daudio/NxBTSettingsR
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxDAudioManagerTest/NxDAudioManagerTest	${IMAGE_ROOTFS}/nexell/daudio/NxDAudioManagerTest
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxQuickRearCam/NxQuickRearCam				${IMAGE_ROOTFS}/nexell/daudio/NxQuickRearCam
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/NxVideoPlayer/NxVideoPlayer				${IMAGE_ROOTFS}/nexell/daudio/NxVideoPlayer

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/podo/podo										${IMAGE_ROOTFS}/podo/
    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/podo/apps/pdwindowcompositor/pdwindowcompositor	${IMAGE_ROOTFS}/podo/apps/pdwindowcompositor
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_daudio_sdk_function;"
