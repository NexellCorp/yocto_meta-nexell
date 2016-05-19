# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import sys
import os

def file_gathering():
    resultpath = ""
    srcpath = []
    try :
        with open("result_path.txt") as data :
            for line in data :
                resultpath = line.strip()
    except IOError :
        print "File open error"
    
    try :
        with open(sys.argv[1]) as data :
            for line in data :
                srcpath.append(line.strip())
                
    except IOError :
        print "File open error"
        
    for i in srcpath :
        temp = "cp "+ i +" " + resultpath
        print temp
        os.system(temp)
    
file_gathering()
