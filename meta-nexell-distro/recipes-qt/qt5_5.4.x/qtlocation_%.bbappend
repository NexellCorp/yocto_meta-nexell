require qt5-nexell.inc

SRC_URI[md5sum] = "9d8bc7815bc789ade028d0d347add6c2"
SRC_URI[sha256sum] = "69336fd11316d5cf715711d6f8e4c7d34251d11cc33d6999beda124148be672d"

EXTRA_QMAKEVARS_PRE = "${@bb.utils.contains('PACKAGECONFIG', 'geoclue', 'CONFIG+=OE_GEOCLUE_ENABLED', '', d)}"
EXTRA_QMAKEVARS_PRE += "${@bb.utils.contains('PACKAGECONFIG', 'gypsy', 'CONFIG+=OE_GYPSY_ENABLED', '', d)}"
