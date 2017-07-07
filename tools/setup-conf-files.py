#!/usr/bin/env python
#

import sys
import os
import fileinput

CONF_DIR_PREFIX = ""
CONF_FILE_LOCAL = "local.conf"
CONF_FILE_BBLAYER = "bblayers.conf"

CONF_LOCAL_CHANGE_NEED_LINE = ['MACHINE ??= "qemux86"',
                               '#DL_DIR ?= "${TOPDIR}/downloads"',
                               '#SSTATE_DIR ?= "${TOPDIR}/sstate-cache"']
CONF_LOCAL_CHANGE_KEYWORD = ['MACHINE ??= ',
			     'DL_DIR ?= "${TOPDIR}/downloads"',
			     'SSTATE_DIR ?= "${TOPDIR}/sstate-cache"']

CONF_BBLAYER_CHANGE_NEED_LINE = ['poky/meta-yocto-bsp']
CONF_BBLAYER_CHANGE_KEYWORD = ['meta-nexell']

CONF_BBMASK = "BBMASK += "

#Not use QT recipes
CONF_BBMASK_NOT_USE_QT = " /meta-nexell/recipes-qt"

#Not use nexell graphics
CONF_BBMASK_NOT_USE_GRAPHICS = " /meta-nexell/recipes-graphics"

#Not use benchmark apps
CONF_BBMASK_NOT_USE_BENCHMARK = " /meta-nexell/recipes-benchmark"

#Use QT recipes
CONF_APPEND_USE_QT = ['PACKAGECONFIG_FB_pn-qtbase = "kms"',
                      'PACKAGECONFIG_DISTRO_pn-qtbase = "accessibility linuxfb eglfs fontconfig alsa tslib gles2 glib examples tools openssl"',
                      'PACKAGECONFIG_append_pn-qtmultimedia  = " gstreamer"',
                      'DISTRO_FEATURES_append = " zeroconf"']

#Use weston & wayland recipes
CONF_APPEND_USE_WSWL = ['DISTRO_FEATURES_remove = " x11"',
                        'DISTRO_FEATURES_append = " systemd wayland opengl"',
                        'REQUIRED_DISTRO_FEATURES = "wayland"',
                        'VIRTUAL-RUNTIME_init_manager = "systemd"',
                        'VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"',
                        'CORE_IMAGE_EXTRA_INSTALL += "wayland weston"',
                        'CORE_IMAGE_EXTRA_INSTALL += "iputils"']

CONF_APPEND_USE_SMARTVOICE = ['DISTRO_FEATURES_remove = " x11"',
                              'DISTRO_FEATURES_append = " systemd wayland opengl"',
                              'REQUIRED_DISTRO_FEATURES = "wayland"',
                              'VIRTUAL-RUNTIME_init_manager = "systemd"',
                              'VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"',
                              'CORE_IMAGE_BASE_INSTALL += "weston weston-init"',
                              'IMAGE_FEATURES += "splash package-management ssh-server-dropbear hwcodecs"']
#xorg/xserver(X11)
CONF_APPEND_USE_X11 = ['DISTRO_FEATURES_remove = " wayland"',
                       'DISTRO_FEATURES_append = " systemd x11 opengl glx zeroconf"',
                       'REQUIRED_DISTRO_FEATURES = "x11"',
                       'VIRTUAL-RUNTIME_init_manager = "systemd"',
                       'VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"',
                       'CORE_IMAGE_EXTRA_INSTALL += "iputils"']

CONF_APPEND_USE_SDL = ['DISTRO_FEATURES_append = " systemd gles2 pulseaudio tslib alsa"',
                       'DISTRO_FEATURES_remove = " directfb x11 wayland opengl"',
                       'VIRTUAL-RUNTIME_init_manager = "systemd"',
                       'VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"']

CONF_WHILTELIST_FLAGS_SET = ['LICENSE_FLAGS_WHITELIST = "commercial"']

CONF_BBMASK_RECIPES_CORE = " /meta-nexell/recipes-core/images"
CONF_BBMASK_S5P4418_NAVI_REF = "/s5p4418-navi-ref"
CONF_BBMASK_S5P4418_DAUDIO_REF = "/s5p4418-daudio-ref"
CONF_BBMASK_S5P4418_AVN_REF = "/s5p4418-avn-ref"
CONF_BBMASK_S5P6818_ARTIK710_RAPTOR = "/s5p6818-artik710-raptor"
CONF_BBMASK_S5P6818_AVN_REF = "/s5p6818-avn-ref"
CONF_BBMASK_S5P4418_SMART_VOICE = "/s5p4418-smart-voice"
CONF_BBMASK_S5P6818_K_BOARD = "/s5p6818-kick-st"

class parsingForpokyfiles():
    linuxMark = '/'
    boardName = ''
    imagetype = ''
    confBBmask = ''
    confAppend = []
    confBBmaskFiles = []

    def __init__(self, arg1, arg2, arg3, arg4) :
        self.currpath = arg1
        self.boardName = arg2
	self.imagetype = arg3
        self.meta_nexell_path = arg4

    def localConfChange(self) :
        localfilepath = self.currpath + "/conf"+self.linuxMark + CONF_FILE_LOCAL
        for line in fileinput.input(localfilepath, inplace = 1):
            print line.replace(CONF_LOCAL_CHANGE_NEED_LINE[0], CONF_LOCAL_CHANGE_KEYWORD[0]+'"'+self.boardName+'"'),

        for line in fileinput.input(localfilepath, inplace = 1):
            print line.replace(CONF_LOCAL_CHANGE_NEED_LINE[1], CONF_LOCAL_CHANGE_KEYWORD[1]),

        for line in fileinput.input(localfilepath, inplace = 1):
            print line.replace(CONF_LOCAL_CHANGE_NEED_LINE[2], CONF_LOCAL_CHANGE_KEYWORD[2]),

        #related QT, wayland/weston
	if self.imagetype == 'tiny' :
            confBBmask = CONF_BBMASK + '"' + CONF_BBMASK_NOT_USE_QT + CONF_BBMASK_NOT_USE_BENCHMARK + CONF_BBMASK_NOT_USE_GRAPHICS + '"'
            confAppend = []
        elif self.imagetype == 'smartvoice' :
            confBBmask = CONF_BBMASK + '"' + CONF_BBMASK_NOT_USE_BENCHMARK + '"'
            confAppend = CONF_APPEND_USE_QT + CONF_APPEND_USE_SMARTVOICE + CONF_WHILTELIST_FLAGS_SET
        elif self.imagetype == 'qt' or self.imagetype == 'daudio' :
            confBBmask = ''
            confAppend = CONF_APPEND_USE_QT + CONF_APPEND_USE_WSWL + CONF_WHILTELIST_FLAGS_SET
        elif self.imagetype == 'tinyui' :
            confBBmask = CONF_BBMASK + '"' +  CONF_BBMASK_NOT_USE_QT + '"'
            confAppend = CONF_APPEND_USE_WSWL
        elif self.imagetype == 'sdl' :
            confBBmask = CONF_BBMASK + '"' +  CONF_BBMASK_NOT_USE_QT + CONF_BBMASK_NOT_USE_BENCHMARK + '"'
            confAppend = CONF_APPEND_USE_SDL + CONF_WHILTELIST_FLAGS_SET # CONF_APPEND_USE_WSWL
        elif self.imagetype == 'sato' :
            #confBBmask = CONF_BBMASK + '"' + CONF_BBMASK_NOT_USE_QT + CONF_BBMASK_NOT_USE_BENCHMARK + '"'
            confBBmask = CONF_BBMASK + '"' + CONF_BBMASK_NOT_USE_QT + '"'
            confAppend = CONF_APPEND_USE_X11 + CONF_WHILTELIST_FLAGS_SET
        else :
            pass

        tempBBmaskFilePath = self.meta_nexell_path + "/recipes-core/images/" + self.boardName
        for root, dirs, files in os.walk(tempBBmaskFilePath) :
            for f in files :
                if self.imagetype in f :
                    pass
                else :
                    print "BBMASKING ==> " + CONF_BBMASK_RECIPES_CORE + "/" + self.boardName + "/" + f
                    self.confBBmaskFiles.append(CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + "/" + self.boardName + "/" + f + '"')

	with open(localfilepath, 'a') as file :
            file.write("#NEXELL appended code\n")

            for i in self.confBBmaskFiles :
                file.write(i+"\n")

	    file.write(confBBmask+"\n")

            for i in confAppend :
                file.write(i+"\n")


    def bblayerConfChange(self) :
        CONF_DIR_PREFIX = self.currpath + "/conf"+self.linuxMark
        for line in fileinput.input(CONF_DIR_PREFIX+CONF_FILE_BBLAYER, inplace = 1):
            print line.replace(CONF_BBLAYER_CHANGE_NEED_LINE[0], CONF_BBLAYER_CHANGE_KEYWORD[0]),

    def getDirMark(self) :
        return self.linuxMark

def main(arg1, arg2, arg3, arg4):
    parseMain = parsingForpokyfiles(arg1, arg2, arg3, arg4)
    parseMain.localConfChange()
#   parseMain.bblayerConfChange()

if __name__ == "__main__":
    try :
        main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
    finally :
        pass
