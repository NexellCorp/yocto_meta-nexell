PACKAGECONFIG_GL = "${@bb.utils.contains('DISTRO_FEATURES', 'opengl', \ 
                                                    bb.utils.contains('DISTRO_FEATURES', 'x11', 'opengl', '', d), '', d)}"