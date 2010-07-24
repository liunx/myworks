#ifndef __FOODBOX_H
#define __FOODBOX_H
#include <linux/cdev.h>
#include <linux/list.h>

#define VOLUME	32

struct food {
	char name[64]; // The food name, bone, cookie or meat
	int weight; // how much the food is
	int time; // how long the food is store
	struct list_head list; // double list
};

struct foodbox {
	int volume; // how much food the foodbox can store
	struct semaphore sem; // keep our data operation safe
	struct cdev cdev;
	// we use the list to connect with food structure
	struct list_head list; 
};

#endif
