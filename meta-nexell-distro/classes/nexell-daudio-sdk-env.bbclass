inherit qmake5_paths

export NX_OE_QMAKE_AR = "${AR}"
export NX_OE_QMAKE_CC = "${CC}"
export NX_OE_QMAKE_CFLAGS = "${CFLAGS}"
export NX_OE_QMAKE_CXX = "${CXX}"
export NX_OE_QMAKE_CXXFLAGS = "${CXXFLAGS}"
export NX_OE_QMAKE_INCDIR_QT = "${STAGING_DIR_TARGET}/${OE_QMAKE_PATH_HEADERS}"
export NX_OE_QMAKE_LDFLAGS = "${LDFLAGS}"
export NX_OE_QMAKE_LIBDIR_QT = "${STAGING_DIR_TARGET}/usr/lib"
export NX_OE_QMAKE_LINK = "${CXX}"
export NX_OE_QMAKE_PATH_HOST_BINS = "${STAGING_BINDIR_NATIVE}${QT_DIR_NAME}"
export NX_OE_QMAKE_QDBUSCPP2XML = "${NX_OE_QMAKE_PATH_HOST_BINS}/qdbuscpp2xml"
export NX_OE_QMAKE_QDBUSXML2CPP = "${NX_OE_QMAKE_PATH_HOST_BINS}/qdbusxml2cpp"
export NX_OE_QMAKE_QT_CONFIG = "${QMAKE_MKSPEC_PATH_TARGET}/mkspecs/qconfig.pri"

export NX_OE_QMAKE_MOC ="${NX_OE_QMAKE_PATH_HOST_BINS}/moc"
export NX_OE_QMAKE_RCC = "${NX_OE_QMAKE_PATH_HOST_BINS}/rcc"
export NX_OE_QMAKE_UIC = "${NX_OE_QMAKE_PATH_HOST_BINS}/uic"

export NX_OE_QMAKESPEC = "${QMAKE_MKSPEC_PATH_NATIVE}/mkspecs/linux-oe-g++"
export NX_OE_XQMAKESPEC = "${QMAKE_MKSPEC_PATH_TARGET}/mkspecs/linux-oe-g++"

export NX_QT_CONF_PATH = "${WORKDIR}/qt.conf"

do_generate_qt_config_file() {
	cat > ${NX_QT_CONF_PATH} << EOF
[Paths]
Prefix = ${OE_QMAKE_PATH_PREFIX}
Headers = ${OE_QMAKE_PATH_HEADERS}
Libraries = ${OE_QMAKE_PATH_LIBS}
ArchData = ${OE_QMAKE_PATH_ARCHDATA}
Data = ${OE_QMAKE_PATH_DATA}
Binaries = ${OE_QMAKE_PATH_BINS}
LibraryExecutables = ${OE_QMAKE_PATH_LIBEXECS}
Plugins = ${OE_QMAKE_PATH_PLUGINS}
Imports = ${OE_QMAKE_PATH_IMPORTS}
Qml2Imports = ${OE_QMAKE_PATH_QML}
Translations = ${OE_QMAKE_PATH_TRANSLATIONS}
Documentation = ${OE_QMAKE_PATH_DOCS}
Settings = ${OE_QMAKE_PATH_SETTINGS}
Examples = ${OE_QMAKE_PATH_EXAMPLES}
Tests = ${OE_QMAKE_PATH_TESTS}
HostBinaries = ${OE_QMAKE_PATH_HOST_BINS}
HostData = ${OE_QMAKE_PATH_HOST_DATA}
HostLibraries = ${OE_QMAKE_PATH_HOST_LIBS}
HostSpec = ${NX_OE_QMAKESPEC}
TartgetSpec = ${NX_OE_XQMAKESPEC}
ExternalHostBinaries = ${OE_QMAKE_PATH_EXTERNAL_HOST_BINS}
Sysroot = ${STAGING_DIR_TARGET}
EOF
}
