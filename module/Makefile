obj-m := AFFL_KM.o
AFFL_KM-objs := AFFL_blockmodule.o AFFL_filemodule.o
all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
