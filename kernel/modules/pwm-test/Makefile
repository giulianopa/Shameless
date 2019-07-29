ARCH=arm
CROSS_COMPILE=arm-none-linux-gnueabi
obj-m := pwm-test.o
KDIR := ${SDK_FOLDER}/kernel/
PWD := $(shell pwd)
ARCH=arm64
CROSS_COMPILE=${SDK_FOLDER}/toolchain/aarch64-linux-gnu/bin/aarch64-linux-gnu-
default:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) SUBDIRS=$(PWD) clean
