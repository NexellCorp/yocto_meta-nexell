#!/usr/bin/env python
# -*- coding: utf-8 -*-
#

import sys
import os

class setup_recipes():
      imagetype = ''
      
      def __init__(self, arg1):
      	  self.imagetype = arg1
	  
      def file_name_change(self):
          os.chdir('../meta-nexell/recipes-core/images')
          for root, dirs, files in os.walk('./') :
              for f in files :
    	          if self.imagetype in f :
	      	      pass
	      	  else :
		      temp = os.path.splitext(f)
		      os.rename(f, temp[0]+temp[1]+"~~")

      def revertname(self) :
          os.chdir('../meta-nexell/recipes-core/images')
          for root, dirs, files in os.walk('./') :
              for f in files :
    	          temp = os.path.splitext(f)
		  if "~~" in temp[1] :
		      os.rename(f, temp[0]+temp[1].strip("~~"))
          
# end of class EasyRamdumpParserRunner
def main(arg1):
    filenamechange = setup_recipes(arg1)
    if arg1=="revert" :
       filenamechange.revertname()
    else :
       filenamechange.file_name_change()
    
if __name__ == "__main__":
    try : 
        main(sys.argv[1])
    finally : 
        pass
