ifneq ($(KERNELRELEASE),)
	obj-m := main.o

else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

mkdev:
	sudo mknod wikipedia b 252 0
	sudo chmod 666 wikipedia

rmdev:
	sudo rm wikipedia

ins:
	sudo insmod main.ko

rmm:
	sudo rmmod main.ko

endif
