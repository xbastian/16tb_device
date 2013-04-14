# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)

BUILD_NUM := "$(shell date +%m%d)"
BUILD_KER := "$(shell uname -r)"
EXTRA_CFLAGS += "-DFLAMES_BUILD=\"$(BUILD_NUM)\""
EXTRA_CFLAGS += "-DFLAMES_KERNEL=\"$(BUILD_KER)\""

#uncomment the following for write back
#EXTRA_CFLAGS += "-DWB_AVAILABLE"

#Uncomment to turn on exclude/include/reset file code
#EXTRA_CFLAGS += -DEXCLUSION_TABLE

obj-m := bigdisk.o

bigdisk-objs := module.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else

ifeq ($(FLAMES_BUILD),)
KERNELDIR ?= "/lib/modules/$(shell uname -r)/build"
PWD := "$(shell pwd)"
FLAMES_BUILD  := "$(shell date +%-m%-d)"
FLAMES_KERNEL := "$(shell uname -r)"
FLAMES_CUSTOM_BUILD  := 1
endif

all: default

default:
ifneq ($(FLAMES_CUSTOM_BUILD),)
	@echo "========== This is a CUSTOM BUILD. Building custom Platform! =========="
endif
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.ko *.o Module.symvers Module.markers *mod.c .*.cmd .tmp_versions *.ko.unsigned modules.order
endif
