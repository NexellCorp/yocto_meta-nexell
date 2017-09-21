require qt5-nexell.inc

SRC_URI[md5sum] = "21317c2d2d9bcebefc77892fc799827b"
SRC_URI[sha256sum] = "3d7b7e65293028f4ea250fd4837a57fffdcfbbf6dfdd421ee46e0d5236e9b9a2"

SRC_URI += " \
    file://0001-qmltestexample-fix-link.patch \
"

EXTRA_QMAKEVARS_PRE = "${@bb.utils.contains('PACKAGECONFIG', 'qtxmlpatterns', 'CONFIG+=OE_QTXMLPATTERNS_ENABLED', '', d)}" 