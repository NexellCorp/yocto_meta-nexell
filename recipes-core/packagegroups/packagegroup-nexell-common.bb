DESCRIPTION = "Nexell libraries"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup
 
WESTON_CONF = " \
    weston-conf \
    "
MX = " \
    mx-1.0 \
    mx-tests \
    "
GSTREAMER10 = " \
    gstreamer1.0 \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    "
NEXELL_LIBS = " \
    gst-plugins-camera \
    gst-plugins-renderer \
    gst-plugins-scaler \
    gst-plugins-video-dec \
    gst-plugins-video-enc \
    gst-plugins-video-sink \
    libdrm \
    libomxil-nx \
    nx-drm-allocator \
    nx-gst-meta \
    nx-renderer \
    nx-scaler \
    nx-v4l2 \
    nx-video-api \
    "
 
RDEPENDS_${PN} = " \
    ${WESTON_CONF} \
    ${MX} \
    ${NEXELL_LIBS} \
    ${GSTREAMER10} \
    "
