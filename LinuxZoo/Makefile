obj-m += food.o

all:
	$(MAKE) -C /media/Liunx/uml/linux-2.6.34 M=`pwd`

clean:
	$(MAKE) -C /media/Liunx/uml/linux-2.6.34 M=`pwd` clean
	$(RM) Module.markers modules.order
