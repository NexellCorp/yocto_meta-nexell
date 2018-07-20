# Created by suker@nexell.co.kr
# Description :  nsih txt file modified, some changed address or file size
import os
import sys
from tempfile import mkstemp
from shutil import copy
from os import fdopen

# fip-loader-usb.img
# fip-nonsecure-usb.bin

NSIH_TXT_DUMMY_FILE_NAME = "nsih-dummy.txt"
NSIH_TXT_OUT_FILE_NAME = "nsih-usbdownload.txt"


def getNSIHSize(fileName):
    temp = os.stat(fileName).st_size
    temp = "%08x" % temp
    return temp


def modNSIHTXT(srcPath, inputFile, loadAddress, startAddress, keyTuple):
    tempFile1, tempFile2 = mkstemp()

    with fdopen(tempFile1, 'wt') as new_file:
        with open(NSIH_TXT_DUMMY_FILE_NAME) as org_file:
            for line in org_file:
                if keyTuple[0][0] in line and keyTuple[0][1] in line:
                    new_file.write(str(getNSIHSize(inputFile)) + "   " +
                                   "".join(line.split('   ')[1:]))
                elif keyTuple[1][0] in line and keyTuple[1][1] in line:
                    new_file.write(loadAddress + "   " +
                                   "".join(line.split('   ')[1:]))
                elif keyTuple[2][0] in line and keyTuple[2][1] in line:
                    new_file.write(startAddress + "   " +
                                   "".join(line.split('   ')[1:]))
                else:
                    new_file.write(line)

    copy(tempFile2, srcPath + "/" + NSIH_TXT_OUT_FILE_NAME)


def main(srcPath, inputFile, loadAddress, startAddress):
    modNSIHTXT(srcPath, inputFile, loadAddress, startAddress,
               (("0x044", "fileSize"),
                ("0x048", "loadAddress"),
                ("0x04C", "startAddress")))


if __name__ == "__main__":
    try:
        main(sys.argv[1]+'/', sys.argv[2], sys.argv[3], sys.argv[4])
    finally:
        pass
