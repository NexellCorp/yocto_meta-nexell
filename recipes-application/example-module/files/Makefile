ARCH := arm
CROSS_COMPILE := ${TARGET_PREFIX}

SRC := $(shell pwd)

obj-m := helloworld-module.o

KDIR := ${STAGING_KERNEL_DIR}

MAKEARCH := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

all:
	$(MAKEARCH) -C $(KERNEL_SRC) M=$(SRC) modules

modules_install:
	$(MAKEARCH) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	$(MAKEARCH) -C $(KERNEL_SRC) M=$(SRC) clean
