PACKAGECONFIG_remove = "dri"
PACKAGECONFIG_remove = "${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'glx', '', d)}"
PACKAGECONFIG_remove = "glamor"