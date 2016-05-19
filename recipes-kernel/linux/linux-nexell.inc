DESCRIPTION = "Linux Kernel"
SECTION = "kernel"
LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

inherit kernel siteinfo
require recipes-kernel/linux/linux-yocto.inc

# Enable OABI compat for people stuck with obsolete userspace
ARM_KEEP_OABI ?= "1"

# Quirk for udev greater or equal 141
UDEV_GE_141 ?= "1"

# Set the verbosity of kernel messages during runtime
# You can define CMDLINE_DEBUG in your local.conf or distro.conf to override this behaviour
#CMDLINE_DEBUG ?= '${@base_conditional("DISTRO_TYPE", "release", "quiet", "debug", d)}'
#CMDLINE_append = " ${CMDLINE_DEBUG}"

# # Set a variable in .configure
# # $1 - Configure variable to be set
# # $2 - value [n/y/value]
# kernel_configure_variable() {
#     # Remove the config
#     CONF_SED_SCRIPT="$CONF_SED_SCRIPT /CONFIG_$1[ =]/d;"
#     if test "$2" = "n"
#     then
#         echo "# CONFIG_$1 is not set" >> ${B}/.config
#     else
#         echo "CONFIG_$1=$2" >> ${B}/.config
#     fi
# }

#do_configure_prepend() {
#     # Clean .config
#    echo "" > ${B}/.config
#    CONF_SED_SCRIPT=""

#     # Set cmdline
# #    kernel_configure_variable CMDLINE "\"${CMDLINE}\""

#     # Keep this the last line
#     # Remove all modified configs and add the rest to .config
#    sed -e "${CONF_SED_SCRIPT}" < '${WORKDIR}/defconfig' >> '${B}/.config'

#    yes '' | oe_runmake oldconfig
#}

# # Automatically depend on lzop-native if CONFIG_KERNEL_LZO is enabled
python () {
     try:
         defconfig = bb.fetch2.localpath('file://defconfig', d)
     except bb.fetch2.FetchError:
         return

     try:
         configfile = open(defconfig)
     except IOError:
         return

     if 'CONFIG_KERNEL_LZO=y\n' in configfile.readlines():
         depends = d.getVar('DEPENDS', False)
         d.setVar('DEPENDS', depends + ' lzop-native')
}