#!/usr/bin/env python
# -*- coding: utf-8 -*-
#

import sys
import os

class setup_recipes():
      imagetype = ''
      MARK_STR = "-not-used"

      def __init__(self, arg1, arg2):
          self.boardname = arg1
      	  self.imagetype = arg2

      def change_dir(self) :
          os.chdir('../meta-nexell/recipes-core/images')

      def file_name_change(self):
          splitFileNameL = []
          for root, dirs, files in os.walk('./') :
              for f in files:
                  splitFileNameL = os.path.splitext(f)[0].split('-')
                  if self.boardname in f and self.imagetype in splitFileNameL :
	      	      pass
		  elif "nexell-image" in f and self.imagetype in splitFileNameL :
		      pass
	      	  else :
		      temp = os.path.splitext(f)
		      os.rename(f, temp[0]+temp[1]+self.MARK_STR)

      def revertname(self) :
          for root, dirs, files in os.walk('./') :
              for f in files :
    	          temp = os.path.splitext(f)
		  if self.MARK_STR in temp[1] :
		      os.rename(f, temp[0]+temp[1].strip(self.MARK_STR))
          
# end of class EasyRamdumpParserRunner
def main(arg1,arg2):
    filenamechange = setup_recipes(arg1,arg2)
    if arg1=="revert" :
       filenamechange.change_dir()
       filenamechange.revertname()
    else :
       filenamechange.change_dir()
       filenamechange.revertname()
       filenamechange.file_name_change()
    
if __name__ == "__main__":
    try : 
        main(sys.argv[1], sys.argv[2])
    finally : 
        pass
