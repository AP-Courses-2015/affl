obj-m := AFFL_KM.o
all:
	$(MAKE) -C /lib/modules/3.13.0-24-generic/build/ M=`pwd` modules
clean:
	$(MAKE) -C /lib/modules/3.13.0-24-generic/build M=`pwd` clean