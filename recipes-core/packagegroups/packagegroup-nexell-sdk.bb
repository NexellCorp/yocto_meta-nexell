SUMMARY = "SDK packages"
DESCRIPTION = "Pacakages for the nexell SDK"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

RDEPENDS_${PN} = " \
    libgcc \
    libgcc-dev \
    libstdc++-dev \
    libsegfault \
    glibc \
    glibc-dbg \
    glibc-dev \
    glibc-utils \
    glibc-thread-db \
    glibc-staticdev \
    linux-libc-headers-dev \
    gdbserver \
    alsa-dev \
    alsa-lib-dev \
    alsa-utils-dev \
    curl-dev \
    i2c-tools-dev \
    freetype-dev \
    lzo-dev \
    opkg-dev \
    libpng-dev \
    readline-dev \
    tslib-dev \
    libusb-compat-dev \
    libusb1-dev \
    zlib-dev \
    ncurses-dev \
    opkg-dev \
    util-linux-dev \
    "