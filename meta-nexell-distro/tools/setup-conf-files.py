#!/usr/bin/env python
#

import sys
import os
import fileinput
import xml.etree.ElementTree as parseXml

CONF_DIR_PREFIX = ""
CONF_FILE_LOCAL = "local.conf"
CONF_FILE_BBLAYER = "bblayers.conf"
TOOLS_CONFIGS_BOARD_PATH = "tools/configs/board"
TOOLS_CONFIGS_IMAGE_PATH = "tools/configs/imagetype"

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

	with open(localfilepath, 'a') as file :
            board_doc = parseXml.parse(self.meta_nexell_path + '/' + TOOLS_CONFIGS_BOARD_PATH + '/' + self.boardName + ".xml")
            root = board_doc.getroot()
            for i in root :
                file.write(i.attrib["text"]+"\n")
                print("local.conf override : " + i.attrib["text"])

            imagetype_doc = parseXml.parse(self.meta_nexell_path + '/' + TOOLS_CONFIGS_IMAGE_PATH + '/' + self.imagetype + ".xml")

            root = imagetype_doc.getroot()
            for i in root :
                #common first, socname need not check
                if i.tag == "common" :
                    for j in i :
                        file.write(j.attrib["text"]+"\n")
                        print("local.conf override : " + j.attrib["text"])

                elif i.tag == self.boardSocName :
                    for j in i :
                        file.write(j.attrib["text"]+"\n")
                        print("local.conf override : " + j.attrib["text"])

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
