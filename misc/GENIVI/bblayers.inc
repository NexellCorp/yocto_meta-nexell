# LAYER_CONF_VERSION is increased each time build/conf/bblayers.conf
# changes incompatibly
LCONF_VERSION = "6"

BBPATH = "${TOPDIR}"
BBFILES ?= ""

BBLAYERS ?= " \
  ${TOPDIR}/../GENIVI/poky/meta \
  ${TOPDIR}/../GENIVI/poky/meta-yocto \
  ${TOPDIR}/../GENIVI/poky/meta-yocto-bsp \
  ${TOPDIR}/../GENIVI/meta-ivi/meta-ivi \
  ${TOPDIR}/../GENIVI/meta-ivi/meta-ivi-bsp \
  ${TOPDIR}/../GENIVI/meta-openembedded/meta-oe \
  ${TOPDIR}/../GENIVI/meta-openembedded/meta-filesystems \
  ${TOPDIR}/../GENIVI/meta-openembedded/meta-ruby \
  ${TOPDIR}/../GENIVI/meta-qt5 \
  ${TOPDIR}/../GENIVI/meta-genivi-dev/meta-genivi-dev \
  ${TOPDIR}/../GENIVI/meta-rust \
  ${TOPDIR}/../GENIVI/meta-oic \
  ${TOPDIR}/../GENIVI/meta-erlang \
  ${TOPDIR}/../GENIVI/meta-rvi \
  "

BBLAYERS_NON_REMOVABLE ?= " \
  ${OEROOT}/meta \
  ${OEROOT}/meta-yocto \
  ${OEROOT}/../meta-ivi/meta-ivi \
  "
