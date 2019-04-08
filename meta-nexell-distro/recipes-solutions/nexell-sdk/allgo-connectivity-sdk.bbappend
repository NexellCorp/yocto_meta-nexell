### Nexell - For Yocto build with using local source, Below lines are auto generated codes

S = "${WORKDIR}/git"
PV = "1.0+EXTERNAL_SRC"

do_myp() {
    rm -rf ${S}
    mv ${WORKDIR}${_SRC_PATH_BY_GEN_} ${S}
    rm -rf ${WORKDIR}/home
}
addtask myp before do_configure after do_unpack


SRC_URI="file://${_SRC_PATH_BY_GEN_}"

_SRC_PATH_BY_GEN_?="/home/kjlee/BSP/linux/yocto-sumo-new/solutions/allgo-connectivity-sdk"