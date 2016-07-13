#!/usr/bin/env python
# -*- coding: CP949 -*-
#

import sys
import fileinput

CONF_DIR_PREFIX = ""
CONF_FILE_LOCAL = "local.conf"
CONF_FILE_BBLAYER = "bblayers.conf"

CONF_LOCAL_CHANGE_NEED_LINE = ['MACHINE ??= "qemux86"',
                               '#DL_DIR ?= "${TOPDIR}/downloads"',
                               '#SSTATE_DIR ?= "${TOPDIR}/sstate-cache"']
CONF_LOCAL_CHANGE_KEYWORD = ['MACHINE ??= ',
			     'DL_DIR ?= "${TOPDIR}/../downloads"',
			     'SSTATE_DIR ?= "${TOPDIR}/../sstate-cache"']

CONF_BBLAYER_CHANGE_NEED_LINE = ['poky/meta-yocto-bsp']
CONF_BBLAYER_CHANGE_KEYWORD = ['meta-nexell']

CONF_BBMASK_APPEND_NOT_USE_QT = ['BBMASK += "|meta-nexell/recipes-qt"']
CONF_BBMASK_APPEND_USE_QT = ['DISTRO_FEATURES_remove = "x11"', 
                             'DISTRO_FEATURES_append = " systemd wayland opengl"',
                             'REQUIRED_DISTRO_FEATURES = "wayland"',
                             'VIRTUAL-RUNTIME_init_manager = "systemd"',
                             'CORE_IMAGE_EXTRA_INSTALL += "wayland weston"',
                             'PACKAGECONFIG_FB_pn-qtbase = "linuxfb"',
                             'PACKAGECONFIG_DISTRO_pn-qtbase = "accessibility eglfs alsa puseaudio fontconfig gles2 glib examples tools openssl"',
			     'LICENSE_FLAGS_WHITELIST = "commercial"']

class parsingForpokyfiles():
    linuxMark = '/'
    boardName = ''
    imagetype = ''
    bbmaskL = []

    def __init__(self, arg1, arg2) :
        self.boardName = arg1
	self.imagetype = arg2
        
    def localConfChange(self) :
        localfilepath = "conf"+self.linuxMark + CONF_FILE_LOCAL
        for line in fileinput.input(localfilepath, inplace = 1): 
            print line.replace(CONF_LOCAL_CHANGE_NEED_LINE[0], CONF_LOCAL_CHANGE_KEYWORD[0]+'"'+self.boardName+'"'),
	    
        for line in fileinput.input(localfilepath, inplace = 1): 
            print line.replace(CONF_LOCAL_CHANGE_NEED_LINE[1], CONF_LOCAL_CHANGE_KEYWORD[1]),
	    
        for line in fileinput.input(localfilepath, inplace = 1): 
            print line.replace(CONF_LOCAL_CHANGE_NEED_LINE[2], CONF_LOCAL_CHANGE_KEYWORD[2]),


	if self.imagetype!='qt' :
            bbmaskL = CONF_BBMASK_APPEND_NOT_USE_QT
        else :
            bbmaskL = CONF_BBMASK_APPEND_USE_QT

	with open(localfilepath, 'a') as file :
	    file.write("#NEXELL appended code\n")
	    for i in bbmaskL :
                file.write(i+"\n")
	
    def bblayerConfChange(self) :
        CONF_DIR_PREFIX = "conf"+self.linuxMark
        for line in fileinput.input(CONF_DIR_PREFIX+CONF_FILE_BBLAYER, inplace = 1): 
            print line.replace(CONF_BBLAYER_CHANGE_NEED_LINE[0], CONF_BBLAYER_CHANGE_KEYWORD[0]),

    def getDirMark(self) :
        return self.linuxMark
       
# end of class EasyRamdumpParserRunner
def main(arg1, arg2):
    parseMain = parsingForpokyfiles(arg1, arg2)
    parseMain.localConfChange()
#   parseMain.bblayerConfChange()
    
if __name__ == "__main__":
    try : 
        main(sys.argv[1], sys.argv[2])
    finally : 
        pass
