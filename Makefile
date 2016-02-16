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
	sudo mknod wikipedia b 123 0
	sudo chmod 666 wikipedia

rmdev:
	sudo rm wikipedia

insmod:
	sudo insmod main.ko major_number=123

rmmod:
	sudo rmmod main.ko
	
reload:
	-@sudo rmmod main.ko
	-@sudo insmod main.ko

endif
