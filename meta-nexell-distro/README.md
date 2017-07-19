p<html xmlns="http://www.w3.org/1999/xhtml" lang="ko"  xml:lang="ko">
<meta http-equiv="Content-Type" Content="text/html; charset=utf-8" />

Nexell Yocto GUIDE

suker : create  2016.05.18 version 0.1<br>
suker : update 2016.05.19 version 0.2<br>
suker : update 2017.07.20 version 1.0

<br>
#Table
[TOC]

<br>
## s5p4418-navi-ref build with pyro, qt image type
### Download
 1. **yocto$ git clone -b pyro git://git.yoctoproject.org/poky.git**
 2. **yocto$ git clone -b pyro https://github.com/meta-qt5/meta-qt5.git**
 3. **yocto$ git clone -b pyro git://git.openembedded.org/openembedded-core**
 4. **yocto$ git clone -b pyro git://git.openembedded.org/meta-openembedded**

### Build
 1. source poky/oe-init-build-env build/build-s5p4418-navi-ref-qt
 2. yocto/build-s5p4418-navi-ref-qt$ **../meta-nexell/tools/envsetup.sh s5p4418-navi-ref qt -1 false no**
 3. yocto/build-s5p4418-navi-ref-qt$ **bitbake nexell-qt**

### Fusing
 1. yocto/build-s5p4418-navi-ref-qt$ ../meta-nexell/tools/copyFilesToOutDir.sh s5p4418-navi-ref qt
 2. yocto/build-s5p4418-navi-ref-qt$ ../meta-nexell/tools/convert_tools/convert_images.sh s5p4418-navi-ref qt
 3. yocto/build-s5p4418-navi-ref-qt$ ../out/result-s5p4418-navi-ref-qt/tools/standalone-fastboot-download.sh

---
<br>
