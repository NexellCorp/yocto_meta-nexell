<html xmlns="http://www.w3.org/1999/xhtml" lang="ko"  xml:lang="ko">
<meta http-equiv="Content-Type" Content="text/html; charset=utf-8" />

Nexell Yocto GUIDE

suker : create  2016.05.18 krogoth base, branch name:nexell<br>
suker : update 2017.07.19 pyro base, branch name:pyro<br>

<br>
#Table
[TOC]

<br>
<br>
** Recommand branch is the 'pyro' **
<br>

### source download
 1. $ **git clone -b pyro git://git.yoctoproject.org/poky.git**
 2. $ **git clone -b pyro http://git.nexell.co.kr:8081/gerrit/nexell/yocto/meta-nexell**
 3. $ **git clone -b pyro https://github.com/meta-qt5/meta-qt5.git**
 4. $ **git clone -b pyro git://git.openembedded.org/openembedded-core**
 5. $ **git clone -b pyro git://git.openembedded.org/meta-openembedded**

<br>
### build env setup
> $ source poky/oe-init-build-env build/build-s5p4418-navi-ref-qt
> build/build-s5p4418-navi-ref-qt$ ../../meta-nexell/meta-nexell-distro/tools/envsetup.sh s5p4418-navi-ref qt -1 no
> build/build-s5p4418-navi-ref-qt$ bitbake nexell-qt

* note: build-{soc name}-{board name}-{image type}
* note: envsetup.sh {machine name} {image type} {core thread build time} {external source using}
*        -1 : full thread
*        no : use not external source

<br>
<br>

### fusing after build
> build/build-s5p4418-navi-ref-qt$ ../../meta-nexell/meta-nexell-distro/tools/copyFilesToOutDir.sh s5p4418-navi-ref qt
> build/build-s5p4418-navi-ref-qt$ cd ../../out/result-s5p4418-navi-ref-qt
> out/result-s5p4418-navi-ref-qt$ ../../meta-nexell/meta-nexell-distro/tools/convert_tools/convert_images.sh s5p4418-navi-ref qt
> out/result-s5p4418-navi-ref-qt$ ./tools/standalone-fastboot-download.sh   #==> fastboot download to target board

<br>
<br>

## Very Easy build & fusing Guide
 1. repo init -u git://git.nexell.co.kr/nexell/yocto/manifest -b yocto-pyro
 2. repo sync
 3. myreroPath$ ./tools/build.sh s5p4418-navi-ref qt
 4. myrepoPath$ ./yocto/out/result-s5p4418-navi-ref-qt/tools/standalone-fastboot-download.sh
 * It's Done!!

<br>
<br>

## Reference
Raspberrypi  :  git clone http://git.yoctoproject.org/git/meta-raspberrypi
Yocto Manual : http://www.yoctoproject.org/docs/current/mega-manual/mega-manual.html#using-bbappend-files
