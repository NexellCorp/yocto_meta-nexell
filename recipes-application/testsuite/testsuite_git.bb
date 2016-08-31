SUMMARY = "Nexell testsuite - allocator-test application"
SECTION = "application"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile;md5=d5743c4d7fa2b466a875bac2c6176aa1"

SRCREV = "92d91d29409ae9f3cfb7bcfa5bc8fd7e218f454c"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/apps/testsuite;protocol=git;branch=nexell"

DEPENDS = "nx-drm-allocator nx-v4l2 nx-renderer nx-scaler nx-gst-meta nx-video-api libdrm-nx libomxil-nx"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
INHIBIT_PACKAGE_STRIP = "1"

inherit autotools

# AUTOTOOLS_SCRIPT_PATH = "${S}/video_api_test"

# do_configure() {
#      #video_api_test
#      cd ${S}/video_api_test
#      NOCONFIGURE=true ./autogen.sh
#      oe_runconf
# }

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
    # #libnx_video_alloc
    # cd ${S}/libnx_video_alloc
    # oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    # oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDE="-I${STAGING_DIR_TCBOOTSTRAP}/usr/include -I${STAGING_INCDIR} -I${S}/libnx_video_alloc/include -I${S}/libnx_video_alloc/src" LIBRARY="-L${S}/libnx_video_alloc/src -lnx_video_alloc" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"

    # ==> Computing transaction...error: Can't install testsuite-NEXELL-0.1@s5p4418_navi_ref: no package provides libavformat.so.56(LIBAVFORMAT_56)
    # below make options are compile-host-path error fixed codes
    #  #video_api_test
    # cd ${S}/video_api_test
    # oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    # oe_runmake CROSS_COMPILE=${TARGET_PREFIX} \
    #                            INCLUDE="-I${STAGING_INCDIR} -I${STAGING_INCDIR}/libdrm" \
    # 			       IBRARY="-L${STAGING_LIBDIR} -L${STAGING_BASELIBDIR}" CC="$CC" \
    # 			       FFMPEG_INC="${S}/video_api_test/src/ffmpeg/ffmpeg-2.8.6-arm32/include" \
    # 			       FFMPEG_LIB="${S}/video_api_test/src/ffmpeg/ffmpeg-2.8.6-arm32/lib" \
    # 			       AM_CPPFLAGS=" \
    #                                        $(WARN_CFLAGS)  \
    # 			                   -I.             \
    # 			                   -I${STAGING_INCDIR} \
    # 			                   -I${STAGING_INCDIR}/libdrm  \
    # 			                   -I${S}/video_api_test/src/include   \
    # 			                   -I${S}/video_api_test/src/ffmpeg/ffmpeg-2.8.6-arm32/include" \
    # 			       video_api_test_LDADD=" \
    # 			                   -L${S}/video_api_test/src/ffmpeg/ffmpeg-2.8.6-arm32/lib \
    # 			                   -lavcodec \
    # 			                   -lavformat \
    # 			                   -lavutil \
    # 			                   -lswresample \
    # 			                   -L${STAGING_LIBDIR} \
    # 			                   -ldrm \
    # 			                   -lnx_video_api"			       
}

do_install() {
    install -d ${D}${bindir}
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

    #video_aapi_test
#    install -m 0755 ${S}/video_api_test/src/video_api_test ${D}${bindir}
}

FILES_${PN} = "${bindir}"
RDEPENDS_${PN} += "libavformat libavcodec"
ALLOW_EMPTY_${PN} = "1"