obj-m += HelloWorld.o

obj-m += procfs4.o
all:
	$(MAKE) -C /lib/modules/`uname -r`/build M=`pwd`

clean:
	$(MAKE) -C /lib/modules/`uname -r`/build M=`pwd` clean
	$(RM) Module.markers modules.order
