#ifndef __FOODBOX_H
#define __FOODBOX_H
#include <linux/cdev.h>

#define MAX_FOOD	32

struct foodbox {
	int foods;
	struct cdev cdev;

};

#endif
