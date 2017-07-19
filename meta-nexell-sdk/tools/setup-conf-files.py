#!/usr/bin/env python
#

import sys
import os
import fileinput

CONF_DIR_PREFIX = ""
CONF_FILE_LOCAL = "local.conf"

CONF_LOCAL_OVERRIDE = ["MACHINE = ",
		       "DL_DIR = \"${TOPDIR}/../downloads\"",
		       "SSTATE_DIR = \"${TOPDIR}/sstate-cache\""]

#Use TINY recipes
CONF_APPEND_USE_TINY_S5P4418 = [
 'SERIAL_CONSOLE = "115200 ttyAMA3"',
 'USE_VT="1"',
 'DISTRO_FEATURES += "sysvinit"'
]

CONF_APPEND_USE_TINY_S5P6818 = [
 'SERIAL_CONSOLE = "115200 ttySAC3"',
 'USE_VT="1"',
 'DISTRO_FEATURES += "sysvinit"'
]

#Use QT recipes
CONF_APPEND_USE_QT = [
 'PACKAGECONFIG_FB_pn-qtbase = "kms"',
 'PACKAGECONFIG_DISTRO_pn-qtbase = "accessibility linuxfb eglfs fontconfig alsa tslib gles2 glib examples tools openssl"',
 'PACKAGECONFIG_append_pn-qtmultimedia  = " gstreamer"',
 'DISTRO_FEATURES_append = " zeroconf"'
]

#xorg/xserver(X11)
CONF_APPEND_USE_X11 = [
 '',
 'VIRTUAL-RUNTIME_init_manager = "systemd"',
 'VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"',
 'CORE_IMAGE_EXTRA_INSTALL += "iputils"',
 'DISTRO_FEATURES_remove = " wayland"',
 'DISTRO_FEATURES_append = " systemd x11 opengl glx zeroconf"',
 '',
]

#Use weston & wayland recipes
CONF_APPEND_USE_WSWL = [
 '',
 'DISTRO_FEATURES_remove = " x11"',
 'DISTRO_FEATURES_append = " systemd wayland opengl"',
 'VIRTUAL-RUNTIME_init_manager = "systemd"',
 'VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"',
 'CORE_IMAGE_EXTRA_INSTALL += "wayland weston"',
 'CORE_IMAGE_EXTRA_INSTALL += "iputils"',
 '',
]

CONF_APPEND_USE_SMARTVOICE = [
 '',
 'DISTRO_FEATURES_remove = " x11"',
 'DISTRO_FEATURES_append = " systemd wayland opengl alsa"',
 'VIRTUAL-RUNTIME_init_manager = "systemd"',
 'VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"',
 'CORE_IMAGE_BASE_INSTALL += "weston weston-init"',
 'IMAGE_FEATURES += "splash package-management ssh-server-dropbear hwcodecs"',
 '',
]

CONF_APPEND_USE_PREFERRED_PROVIDER_QT = [
 '',
 'PREFERRED_PROVIDER_virtual/libgles1 = "nexell-drm-mali-qt"',
 'PREFERRED_PROVIDER_virtual/libgles2 = "nexell-drm-mali-qt"',
 'PREFERRED_PROVIDER_virtual/egl = "nexell-drm-mali-qt"',
 'PREFERRED_PROVIDER_virtual/libopencl = "nexell-drm-mali-qt"',
 'PREFERRED_PROVIDER_virtual/libgl = "mesa"',
 'PREFERRED_PROVIDER_virtual/mesa = "mesa"',
 'PREFERRED_PROVIDER_libgbm = "nexell-drm-mali-qt"',
 'PREFERRED_PROVIDER_libgbm-dev = "nexell-drm-mali-qt"',
 '',
]

CONF_APPEND_USE_PREFERRED_PROVIDER_X11 = [
 '',
 'PREFERRED_PROVIDER_virtual/xserver = "xserver-xorg"',
 'XSERVER = "xserver-xorg \ ',
 '           xf86-input-evdev \ ',
 '           xf86-input-mouse \ ',
 '           xf86-video-fbdev \ ',
 '           xf86-input-keyboard"',
 '',
 'PREFERRED_PROVIDER_virtual/libgles1 = "nexell-drm-mali-sato"',
 'PREFERRED_PROVIDER_virtual/libgles2 = "nexell-drm-mali-sato"',
 'PREFERRED_PROVIDER_virtual/egl = "nexell-drm-mali-sato"',
 'PREFERRED_PROVIDER_virtual/libopencl = "nexell-drm-mali-sato"',
 'PREFERRED_PROVIDER_virtual/libgl = "mesa-gl"',
 'PREFERRED_PROVIDER_virtual/mesa = "mesa-gl"',
 'PREFERRED_PROVIDER_libgbm = "nexell-drm-mali-sato"',
 'PREFERRED_PROVIDER_libgbm-dev = "nexell-drm-mali-sato"',
 '',
]

CONF_APPEND_USE_SDL = [
 '',
 'DISTRO_FEATURES_append = " systemd gles2 pulseaudio tslib alsa"',
 'DISTRO_FEATURES_remove = " directfb x11 wayland opengl"',
 'VIRTUAL-RUNTIME_init_manager = "systemd"',
 'VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"',
 '',
]

CONF_WHILTELIST_FLAGS_SET = ['LICENSE_FLAGS_WHITELIST = "commercial"']

class parsingForpokyfiles():
    linuxMark = '/'
    boardName = ''
    imagetype = ''
    conf_NOT_USE = ''
    conf_USE = []
    confBBmaskFiles = []

    def __init__(self, arg1, arg2, arg3, arg4, arg5) :
        self.currpath = arg1
        self.boardName = arg2
	self.imagetype = arg3
        self.meta_nexell_path = arg4
        self.externalsrc_using = arg5
        self.boardSocName = self.boardName.split('-')[0]

    def localConfChange(self) :
        localfilepath = self.currpath + "/conf"+self.linuxMark + CONF_FILE_LOCAL

        #related QT, wayland/weston
	if self.imagetype == 'tiny' :
            if self.boardSocName == 's5p4418' :
                conf_USE = CONF_APPEND_USE_TINY_S5P4418
            else :
                conf_USE = CONF_APPEND_USE_TINY_S5P6818
        elif self.imagetype == 'smartvoice' :
            conf_USE = CONF_APPEND_USE_QT + CONF_APPEND_USE_SMARTVOICE + CONF_APPEND_USE_PREFERRED_PROVIDER_QT + CONF_WHILTELIST_FLAGS_SET
        elif self.imagetype == 'qt' :
            conf_USE = CONF_APPEND_USE_QT + CONF_APPEND_USE_WSWL + CONF_APPEND_USE_PREFERRED_PROVIDER_QT + CONF_WHILTELIST_FLAGS_SET
        elif self.imagetype == 'sdl' :
            conf_USE = CONF_APPEND_USE_SDL + CONF_WHILTELIST_FLAGS_SET # CONF_APPEND_USE_WSWL
        elif self.imagetype == 'sato' :
            conf_USE = CONF_APPEND_USE_X11 + CONF_APPEND_USE_PREFERRED_PROVIDER_X11 + CONF_WHILTELIST_FLAGS_SET
        else :
            pass

	with open(localfilepath, 'a') as file :
            file.write(CONF_LOCAL_OVERRIDE[0]+"\""+self.boardName+"\""+"\n")
            file.write(CONF_LOCAL_OVERRIDE[1]+"\n")
            file.write(CONF_LOCAL_OVERRIDE[2]+"\n")

            for i in conf_USE :
                file.write(i+"\n")

            if self.externalsrc_using == 'EXTERNALSRC_USING' :
                file.write('INHERIT += "externalsrc"'+"\n")

    def getDirMark(self) :
        return self.linuxMark

def main(arg1, arg2, arg3, arg4, arg5):
    parseMain = parsingForpokyfiles(arg1, arg2, arg3, arg4, arg5)
    parseMain.localConfChange()

if __name__ == "__main__":
    try :
        main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
    finally :
        pass
