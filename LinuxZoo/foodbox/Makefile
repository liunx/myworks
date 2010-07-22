obj-m += foodbox.o

all:
	$(MAKE) -C /media/Liunx/uml/linux-2.6.34 M=`pwd`
	gcc addfood.c -o addfood

clean:
	$(MAKE) -C /media/Liunx/uml/linux-2.6.34 M=`pwd` clean
	$(RM) Module.markers modules.order
	rm addfood
