#!/usr/bin/env python
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

CONF_BBMASK = "BBMASK += "
#Not use QT recipes
CONF_BBMASK_NOT_USE_QT = " /meta-nexell/recipes-qt"
#Not use weston & wayland recipes
CONF_BBMASK_NOT_USE_WSWL = " /meta-nexell/recipes-graphics /meta-nexell/recipes-benchmark"
#Use QT recipes
CONF_APPEND_USE_QT = ['PACKAGECONFIG_FB_pn-qtbase = "kms"',
                      'PACKAGECONFIG_DISTRO_pn-qtbase = "accessibility linuxfb eglfs fontconfig alsa tslib gles2 glib examples tools openssl"',
                      'PACKAGECONFIG_append_pn-qtmultimedia  = " gstreamer"',
                      'LICENSE_FLAGS_WHITELIST = "commercial"']
#Use weston & wayland recipes
CONF_APPEND_USE_WSWL = ['DISTRO_FEATURES_remove = " x11"',
                        'DISTRO_FEATURES_append = " systemd wayland opengl"',
                        'REQUIRED_DISTRO_FEATURES = "wayland"',
                        'VIRTUAL-RUNTIME_init_manager = "systemd"',
                        'VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"',
                        'CORE_IMAGE_EXTRA_INSTALL += "wayland weston"',
                        'CORE_IMAGE_EXTRA_INSTALL += "iputils"']

CONF_BBMASK_RECIPES_CORE = " /meta-nexell/recipes-core/images"
CONF_BBMASK_S5P4418_NAVI_REF = "/s5p4418-navi-ref"
CONF_BBMASK_S5P4418_AVN_REF = "/s5p4418-avn-ref"
CONF_BBMASK_S5P6818_ARTIK710_RAPTOR = "/s5p6818-artik710-raptor"
CONF_BBMASK_S5P6818_AVN_REF = "/s5p6818-avn-ref"

class parsingForpokyfiles():
    linuxMark = '/'
    boardName = ''
    imagetype = ''
    confBBmask = ''
    confBBmask_core = ''
    confAppend = []

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


        #related QT, wayland/weston
	if self.imagetype == 'tiny' :
            confBBmask = CONF_BBMASK + '"' + CONF_BBMASK_NOT_USE_QT + CONF_BBMASK_NOT_USE_WSWL + '"'
            confAppend = []
        elif self.imagetype == 'qt' :
            confBBmask = ''
            confAppend = CONF_APPEND_USE_QT + CONF_APPEND_USE_WSWL
        elif self.imagetype == 'tinyui' :
            confBBmask = CONF_BBMASK + '"' +  CONF_BBMASK_NOT_USE_QT + '"'
            confAppend = CONF_APPEND_USE_WSWL
        elif self.imagetype == 'sato' :
            confBBmask = CONF_BBMASK + '"' + CONF_BBMASK_NOT_USE_QT + CONF_BBMASK_NOT_USE_WSWL + '"'
            confAppend = []
        else :
            pass

        #related recipes-core
        if self.boardName == 's5p4418-navi-ref' :
            confBBmask_core  = CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P4418_AVN_REF + '"\n'
            confBBmask_core += CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P6818_ARTIK710_RAPTOR + '"\n'
            confBBmask_core += CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P6818_AVN_REF + '"\n'
        elif self.boardName == 's5p4418-avn-ref' :
            confBBmask_core  = CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P4418_NAVI_REF + '"\n'
            confBBmask_core += CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P6818_ARTIK710_RAPTOR + '"\n'
            confBBmask_core += CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P6818_AVN_REF + '"\n'
        elif self.boardName == 's5p6818-artik710-raptor' :
            confBBmask_core  = CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P4418_AVN_REF + '"\n'
            confBBmask_core += CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P4418_NAVI_REF + '"\n'
            confBBmask_core += CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P6818_AVN_REF + '"\n'            
        elif self.boardName == 's5p6818-avn-ref' :
            confBBmask_core  = CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P4418_AVN_REF + '"\n'
            confBBmask_core += CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P6818_ARTIK710_RAPTOR + '"\n'
            confBBmask_core += CONF_BBMASK + '"' + CONF_BBMASK_RECIPES_CORE + CONF_BBMASK_S5P4418_NAVI_REF + '"\n'
        else :
            pass

	with open(localfilepath, 'a') as file :
	    file.write("\n#NEXELL appended code\n")
	    file.write(confBBmask+"\n")
            file.write(confBBmask_core+"\n")
            for i in confAppend :
                file.write(i+"\n")
	
    def bblayerConfChange(self) :
        CONF_DIR_PREFIX = "conf"+self.linuxMark
        for line in fileinput.input(CONF_DIR_PREFIX+CONF_FILE_BBLAYER, inplace = 1): 
            print line.replace(CONF_BBLAYER_CHANGE_NEED_LINE[0], CONF_BBLAYER_CHANGE_KEYWORD[0]),

    def getDirMark(self) :
        return self.linuxMark
       
def main(arg1, arg2):
    parseMain = parsingForpokyfiles(arg1, arg2)
    parseMain.localConfChange()
#   parseMain.bblayerConfChange()
    
if __name__ == "__main__":
    try : 
        main(sys.argv[1], sys.argv[2])
    finally : 
        pass
