ifneq ($(KERNELRELEASE),)
	obj-m := main.o

else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

mkdev:
	sudo mknod wikipedia b 137 0
	sudo chmod 666 wikipedia

rmdev:
	sudo rm wikipedia

endif
