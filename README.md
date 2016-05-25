<html xmlns="http://www.w3.org/1999/xhtml" lang="ko"  xml:lang="ko">
<meta http-equiv="Content-Type" Content="text/html; charset=utf-8" />

Nexell Yocto GUIDE

suker : create  2016.05.18 version 0.1<br>
suker : update 2016.05.19 version 0.2


<br>
#Table
[TOC]

<br>
## artik710-raptor build with Krogoth - Summary
### setup & build
 1. krogoth$ **git clone -b krogoth git://git.yoctoproject.org/poky.git**
 2. krogoth$ **git clone git://git.nexell.co.kr/nexell/yocto/krogoth/meta-nexell**
 3. krogoth$ **source poky/oe-init-build-env build-artik710-raptor**
 4. krogoth/build-artik710-raptor$ **../meta-nexell/tools/envsetup.sh artik710-raptor tiny**
 5. krogoth/build-artik710-raptor$ **bitbake nexell-artik710-raptor-tiny**

### target download after build
 1. krogoth/build-artik710-raptor$ **../meta-nexell/tools/result-file-move.sh artik710-raptor tiny** 
 2. krogoth/build-artik710-raptor$ **cd ../result-artik710-raptor**
 3. **krogoth/result-artik710-raptor$ ../meta-nexell/tools/convert_images.sh tiny**
 4. krogoth/result-artik710-raptor$ 
   **../meta-nexell/tools/update.sh -p ../meta-nexell/tools/partmap_emmc.txt -r . -t boot**
       <b>또는</b>  
    **../meta-nexell/tools/update_artik710_raptor_bootonly.sh**
 5. krogoth/result-artik710-raptor$ **../meta-nexell/tools/run-uboot-by-usb.sh**

---
<br>
<br>
<br>

##artik710-raptor build with Krogoth - Detail
###1. download krogoth
description : 최신 버전의 krogoth를 download.
> krogoth$ git clone -b krogoth git://git.yoctoproject.org/poky.git


###2. download meta-nexell
description : meta-nexell layer를 download.
> krogoth$ git clone git://git.nexell.co.kr/nexell/yocto/krogoth/meta-nexell

<b><i>위 1, 2번 download 는 처음 한번만 수행하면된다.</i></b>
<br>
###3. build env setup
description : <b><i>build dir  name  은 자유롭게 사용해도 상관없다.
> krogoth$ source poky/oe-init-build-env build-artik710-raptor


###4.  modify conf/local.conf & conf/bblayer.conf
../meta-nexell/tools/envsetup.sh artik710-raptor 
위 command를 실행하면 아래와 같이 .conf file이 자동으로 바뀌게 된다.
>./conf/local.conf <br>
	>> -	MACHINE ??= "qemux86"                              ⇒   **`"artik710-raptor"`** <br>
	>> -	#DL_DIR ?= "\${TOPDIR}/downloads"           ⇒ **`DL_DIR ?= "\${TOPDIR}/../downloads"`** <br>
	>> -	#SSTATE_DIR ?= "\${TOPDIR}/sstate-cache" 
	  ⇒ **`SSTATE_DIR ?= "\${TOPDIR}/../sstate-cache"`** <br>

>./conf/bblayer.conf <br>
	>> -   BBLAYERS ?= " \ <br>
     /home/suker/YoctoPrj/krogoth/poky/meta \ <br>
     /home/suker/YoctoPrj/krogoth/poky/meta-poky \ <br>
     <del>/home/suker/YoctoPrj/krogoth/poky/meta-poky-bsp \</del>      <br></br>
    <B>/home/suker/YoctoPrj/krogoth/`meta-nexell` \</B> <br>
    "    <br>

	
###5. build start
description : recipes-core/image 의 .bb file name 으로 recipe를 지정하여 build 하도록 한다.
> ./krogoth/build-artik710-raptor$ **bitbake nexell-artik710-raptor-tiny**



###6. deploy images download to target-board
> result files 위치 :
>> **./krogoth/build-artik710-raptor/tmp/deploy/images/artik710-raptor$**
>
>  description :<br>
>  Kernel Image 및 .dtb , rootfs, kernel module file은 위 deploy 디렉토리에 생성되지만
>  u-boot.bin, bl1-raptor.bin 은 다른 work 폴더에 생성된다.
>  Host PC 의 core 갯수만큼 parellel build를 하게 되는데, 빌드 시점및 binary 생성 시점이 늘 똑같지 
>  않기 때문에 한군데에 deploy 하기 어렵다.
>  따라서 build-artik710-raptor/tmp/work 디렉토리의 image_where.txt에  u-boot.bin , bl1-raptor.bin 의 위치를 저장하도록 하였다. 


###7. target-board download after out files move to temporary dir
description : result_path.txt (dir: /krogoth/result-artik710-raptor)와 ./tmp/work/image_where.txt 를
사용하여 각각의 binary imagee file들을 result-artik710-raptor 디렉토리에 모은다.
> krogoth/build-artik710-raptor$ ../meta-nexell/tools/result-file-move.sh artik710-raptor tiny

> krogoth/build-artik710-raptor$ cd ../result-artik710-raptor

description : ramdisk, boot.img 를 만들기 위한 과정이다.
> krogoth/result-artik710-raptor$ ./convert_images.sh tiny

description : download 방법은 기존 artik710-raptor board의 방법과 동일하다.
> krogoth/result-artik710-raptor$ 
../meta-nexell/tools/update.sh -p ../meta-nexell/tools/partmap_emmc.txt -r . -t boot  <br>
또는 <br>
../meta-nexell/tools/update_artik710_raptor_bootonly.sh

description : usb download 역시 기존 방법과 동일하다.
> krogoth/result-artik710-raptor$ ../meta-nexell/tools/run-uboot-by-usb.sh


###8. 기타 
- clean build : bitbake -c cleanall nexell-artik710-raptor-tiny
- 가장 확실한 clean build : build-artik710-raptor dir 의 cache, tmp 폴더 삭제 후 rebuild


---
<br>

## TroubleShooting
###ERROR: No checksum specified
- md5 checksum 관련 문제로써 bitbake build시 source를 git clone 하게되는데, 이때 해당 git 에 존재하는 COPYING 또는 LICENCE 파일의 md5sum 값이 miss match 한 경우 발생한다. 
- **suker@sukernexell:~/temp/artikKernel/kernel-4.1.15$ md5sum COPYING **
**d7810fab7487fb0aad327b76f1be7cd7  COPYING**

	```{.no-highlight}
	- linux-artik7_git.bb - 
	
	(snip)
	SRCREV = "de3c20ee5f02e990000df04fc2938e4beca88970"
	LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

	SRC_URI[md5sum] = "dbb4a2b9d337a90a304d3e30b86b8d27"
	SRC_URI[sha256sum] = "421ddaca0fa29bf8be2affea6fb020ab5bcfb67f368d8f7bba8d8cfd8c88a61f"

	SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=artik"
	(snip)
	```

---

###SRCREV
 - 위 코드에서 SRCREV 은 git clone 시 받을 commit ID 이다.
 - SRCREV 설정이 맞지않으면 엉뚱한 version의 source로 빌드하게 된다.

---

###aarch64-linux-gnu-ld: unrecognized option '-Wl,-O1'   ,  link error!!
 - build 할 source 종류에 따라 발생할 수 있다. artik7의 경우  kernel compile time에 aarch64-poky-gnu tool-chain 에서 위 option 을 처리하지 못한다.
	```{.no-highlight}
	- linux-artik7_git.bb -
	
	(snip)
	do_compile() {
	   oe_runmake ARCH=arm64 distclean
	   local dts_file=${S}/arch/arm64/boot/dts/nexell/s5p6818-artik710.dtsi

	   oe_runmake ARCH=arm64 artik710_raptor_defconfig

	   /#TARGET_LDFLAGS="-Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed"
	   export LDFLAGS="-O1 --hash-style=gnu --as-needed"

	   local memreserve_size=0x7ed00000
	   local reg_size=0x3ee00000
	 (snip)
	```

---

### krogoth vs jethro
 - jethro는 문제되지 않았으나 krogoth 에서는 .bb file에 license field 를 명시해야한다.

	```{.no-highlight}
	LICENSE = "GPLv2"   # or LICENSE = "GPLv2+"
   ```

---


### inherit vs require vs include
 -  http://www.yoctoproject.org/docs/2.1/mega-manual/mega-manual.html
Keywords: BitBake recipes use only a few keywords. You use keywords to include common functions (inherit), load parts of a recipe from other files (include and require) and export variables to the 
environment (export).  
 - inherit은 common function(classes)을 상속받을때, include 와 require는 그외에 다른 class나 .inc file을 가져올때 사용한다. 
	> require linux-nexell.inc
		inherit linux-nexell-base
		
 - include 를 사용할때 해당 file이 없어도 error는 발생하지 않지만 require는 해당 file이 없으면 error 가 발생한다.

---

###bitbake options
 > -k : errorr 가 나더라도 계속 빌드<br>
-f : compile task 가 stamp를 무시하고 강제로 빌드한다.<br>
-c : 특정 task 실행.  bitbake -c compile &lt;recipe name><br>
-v : 자세한 로그<br>
-D : debug level 증가, -DD or -DDD 처럼 여러번 명시 가능<br>
-n : dry-run 아무것도 실행하지 않고 그냥 훑어보기만 한다.<br>
-p : parsing만 하고 종료한다.<br>
-e : 환경 변수 얻기<br>
-g : 의존성 그래프 생성<br>

---

###oe_runmake
 - conf/local.con 에 아래처럼 병렬 빌드 option을 지정하지 않으면 system 설정값대로 빌드된다.
  - BB_NUMBER_THREADS ?= "1.5 \* 코어 개수"
  - PARALLEL_MAKE ?= "-j 2 \* 코어 개수"
 - 따라서 "-j 1" 빌드가 필요한 경우 아래처럼 사용해야한다.

```{.no-highlight}
	- bl1-artik7.bb -

	(snip)
	do_compile () {
	    oe_runmake -j 1 BOARD="RAPTOR"
	}
	(snip)
```

---

### WORKING dir
 - task별로 working dir이 다르다 따라서 .bb 에서 task function 구현시 주의해야한다.<br>
>- u-boot-artik7<br>
	u-boot -- WORKDIR ==>      /tmp/work/artik710_raptor-poky-linux/u-boot-artik7/git-r0<br>
	u-boot -- D ==>            /tmp/work/artik710_raptor-poky-linux/u-boot-artik7/git-r0/image<br>
	u-boot -- BASE_WORKDIR ==> /tmp/work<br>
 - linux-artik7_git<br>
	kernel -- WORKDIR ==>      /tmp/work/artik710_raptor-poky-linux/linux-artik7/1_4.1.0322-r0<br>
	kernel -- D ==>            /tmp/work/artik710_raptor-poky-linux/linux-artik7/1_4.1.0322-r0/image<br>
	kernel -- BASE_WORKDIR ==> /tmp/work<br>
	kernel -- DEPLOYDIR ==>    /tmp/work/artik710_raptor-poky-linux/linux-artik7/1_4.1.0322-r0/deploy-linux-artik7<br>

#  

---
<br>

## Reference
Raspberrypi  :  git clone http://git.yoctoproject.org/git/meta-raspberrypi
Yocto Manual : http://www.yoctoproject.org/docs/current/mega-manual/mega-manual.html#using-bbappend-files