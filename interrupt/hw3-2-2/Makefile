obj-m +=lab1_workqueue_shared.o

KDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o
