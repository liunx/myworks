obj-m += HelloWorld.o

obj-m += procfs4.o
all:
	$(MAKE) -C /media/Liunx/uml/linux-2.6.34 M=`pwd`

clean:
	$(MAKE) -C /media/Liunx/uml/linux-2.6.34 M=`pwd` clean
	$(RM) Module.markers modules.order
