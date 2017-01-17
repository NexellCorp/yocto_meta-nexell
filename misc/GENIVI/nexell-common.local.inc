include local.inc

CORE_IMAGE_EXTRA_INSTALL += "wayland weston "

MULTI_PROVIDER_WHITELIST += "virtual/libgl virtual/egl virtual/libgles1 virtual/libgles2"

MACHINE_FEATURES += "keyboard mouse touchscreen"

PACKAGECONFIG_DISTRO_pn-qtbase = " accessibility linuxfb fontconfig alsa tslib gles2 glib examples tools openssl"
PACKAGECONFIG_append_pn-qtmultimedia  = " gstreamer"
VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"

# for Wayland/Weston
PREFERRED_PROVIDER_virtual/libgles1 = "nexell-drm-mali"
PREFERRED_PROVIDER_virtual/libgles2 = "nexell-drm-mali"
PREFERRED_PROVIDER_virtual/egl = "nexell-drm-mali"
PREFERRED_PROVIDER_virtual/libopencl = "nexell-drm-mali"
PREFERRED_PROVIDER_virtual/libgl = "mesa"
PREFERRED_PROVIDER_virtual/mesa = "mesa"
PREFERRED_PROVIDER_libgbm = "nexell-drm-mali"
PREFERRED_PROVIDER_libgbm-dev = "nexell-drm-mali"

NEXELL_COMMON_INSTALL = " \
    kernel-modules \
    android-tools \
    nexell-drm-mali \
    iputils \
    udev-extraconf \
    perf \
    "

TOUCH_IMAGE_INSTALL = " \
    tslib \
    tslib-conf \
    tslib-tests \
    tslib-calibrate \
    tslib-nexell \
    "

QT_ALSA_IMAGE_INSTALL = " \
    alsa-utils \
    alsa-lib \
    "

QT_ALSA_EXTRA_IMAGE_INSTALL = " \
    alsa-utils-alsamixer \
    alsa-utils-midi \
    alsa-utils-aplay \
    alsa-utils-amixer \
    alsa-utils-aconnect \
    alsa-utils-iecset \
    alsa-utils-speakertest \
    alsa-utils-aseqnet \
    alsa-utils-aseqdump \
    alsa-utils-alsactl \
    "

QT_MULTIMEDIA = " \
    qtmultimedia \
    qtmultimedia-plugins \
    qtmultimedia-qmlplugins \
    "

NEXELL_MULTIMEDIA = " \
    tinyalsa \
    "

IMAGE_INSTALL_append = " \
    ${NEXELL_COMMON_INSTALL} \
    ${TOUCH_IMAGE_INSTALL} \
    ${QT_ALSA_IMAGE_INSTALL} \
    ${QT_ALSA_EXTRA_IMAGE_INSTALL} \
    ${QT_MULTIMEDIA} \
    packagegroup-nexell-common \
    ${NEXELL_MULTIMEDIA} \
    "
