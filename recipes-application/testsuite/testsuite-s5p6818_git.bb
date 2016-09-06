inherit linux-nexell-base

SUMMARY = "Nexell testsuite - allocator-test application"
SECTION = "application"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile;md5=d5743c4d7fa2b466a875bac2c6176aa1"

SRCREV = "24cce8f055a4a14b39093f965654f7d63e7eefb0"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/apps/testsuite;protocol=git;branch=nexell"

DEPENDS = "nx-drm-allocator nx-v4l2 nx-renderer nx-scaler nx-gst-meta nx-video-api libdrm-nx libomxil-nx"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
INHIBIT_PACKAGE_STRIP = "1"

inherit autotools pkgconfig

AUTOTOOLS_SCRIPT_PATH = "${S}/video_api_test"

ARCH_TYPE = "${@get_kernel_arch(d,"${TARGET_PREFIX}")}"

EXTRA_OECONF = " \
    '--prefix=${STAGING_DIR_HOST}' \
    "
   
do_configure() {
    #video_api_test
    cd ${S}/video_api_test
    NOCONFIGURE=true ./autogen.sh
    oe_runconf
}

do_compile() {
    #allocator_test
    cd ${S}/allocator_test
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR}" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"
        
    #camera_test
    cd ${S}/camera_test
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR}" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"    

    #camera_test_onedevice
    cd ${S}/camera_test_onedevice
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR}" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"

    #dp_cam_test
    cd ${S}/dp_cam_test
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR} -I${STAGING_INCDIR}/libdrm -I${STAGING_INCDIR}/nexell" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"

    #dp_cam_test_onedevice
    cd ${S}/dp_cam_test_onedevice
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR} -I${STAGING_INCDIR}/nexell -I${STAGING_INCDIR}/libdrm" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"    

    #scaler_test
    cd ${S}/scaler_test
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR} -I${STAGING_INCDIR}/libdrm -I${STAGING_INCDIR}/libkms" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"

    #dp_clipper_decimator_test
    cd ${S}/dp_clipper_decimator_test
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR} -I${STAGING_INCDIR}/libdrm -I${STAGING_INCDIR}/nexell" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"

    #dp_decimator_test
    cd ${S}/dp_decimator_test
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR} -I${STAGING_INCDIR}/libdrm -I${STAGING_INCDIR}/nexell" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"

    #  ==> soft/hard floating problem... can't compile
    #libnx_video_alloc
    cd ${S}/libnx_video_alloc
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDE="-I${STAGING_DIR_TCBOOTSTRAP}/usr/include -I${S}/libnx_video_alloc/include -I${S}/libnx_video_alloc/src" LIBRARY="-L${S}/libnx_video_alloc/src -lnx_video_alloc" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"

    # ==> Computing transaction...error: Can't install testsuite-NEXELL-0.1@s5p4418_navi_ref: no package provides libavformat.so.56(LIBAVFORMAT_56)
    # poky native : libavformat.so.54.20.4
    #             : libavcodec.so.54.35.0
    #
    #below make options are compile-host-path error fixed codes
    #video_api_test
    cd ${S}/video_api_test
   
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} \
    			       AM_CPPFLAGS+="$(WARN_CFLAGS) \
    			                   -I. \
    			                   -I${STAGING_INCDIR} \
    			                   -I${STAGING_INCDIR}/libdrm \
    			                   -I${S}/video_api_test/src/include \
    			                   -I${S}/video_api_test/src/ffmpeg/ffmpeg-2.8.6-${ARCH_TYPE}/include" \
    			       video_api_test_LDADD+="-L${S}/video_api_test/src/ffmpeg/ffmpeg-2.8.6-${ARCH_TYPE}/lib \
    			                   -lavcodec \
    			                   -lavformat \
    			                   -lavutil \
    			                   -lswresample \    			                   
    			                   -ldrm \    			              
                                           -lnx_video_api          \
                                           -lnx_drm_allocator      \
                                           -lnx_v4l2"
}

do_install() {
    install -d ${D}${bindir}
    install -d ${D}${libdir}
    
    #allocator-test
    install -m 0755 ${S}/allocator_test/allocator-test ${D}${bindir}

    #camera_test
    install -m 0755 ${S}/camera_test/nx-camera-test ${D}${bindir}
    
    #camera_test_onedevice
    install -m 0755 ${S}/camera_test_onedevice/nx-camera-test-onedevice ${D}${bindir}    
    
    #dp_cam_test
    install -m 0755 ${S}/dp_cam_test/dp_cam_test ${D}${bindir}

    #dp_cam_test_onedevice
    install -m 0755 ${S}/dp_cam_test_onedevice/dp_cam_test_onedevice ${D}${bindir}
    
    #--- gsttest ---
    install -d ${D}${bindir}/gsttest
    install -d ${D}${bindir}/gsttest/nxscaler
    install -m 0755 ${S}/gsttest/gsttest.sh ${D}${bindir}/gsttest
    install -m 0755 ${S}/gsttest/nxscaler/* ${D}${bindir}/gsttest/nxscaler

    #scaler_test
    install -m 0755 ${S}/scaler_test/nx-scaler-test ${D}${bindir}

    #dp_clipper_decimator_test
    install -m 0755 ${S}/dp_clipper_decimator_test/dp-clipper-decimator-test ${D}${bindir}
    
    #dp_decimator_test
    install -m 0755 ${S}/dp_decimator_test/dp-decimator-test ${D}${bindir}

    #libnx_video_alloc
#    install -m 0644 ${S}/libnx_video_alloc/src/libnx_video_alloc.a ${D}${libdir}
#    install -m 0755 ${S}/libnx_video_alloc/test/test_video_alloc ${D}${bindir}
    
    #video_api_test
    install -m 0755 ${S}/video_api_test/src/video_api_test ${D}${bindir}
    cp -a ${S}/video_api_test/src/ffmpeg/ffmpeg-2.8.6-${ARCH_TYPE}/lib/*.* ${D}${libdir}
    #install -m 0644 ${S}/video_api_test/src/ffmpeg/ffmpeg-2.8.6-${ARCH_TYPE}/lib/*.so ${D}${libdir}
    #install -m 0644 ${S}/video_api_test/src/ffmpeg/ffmpeg-2.8.6-${ARCH_TYPE}/lib/*.so.* ${D}${libdir}
}

PREFERRED_VERSION_libavcodec = "56.60.100"
PREFERRED_VERSION_libavformat = "56.40.101"
INSANE_SKIP_${PN} = "ldflags"
FILES_${PN} = "${bindir} ${libdir}"
#RDEPENDS_${PN} += "libavformat libavcodec"
ALLOW_EMPTY_${PN} = "1"