#ifndef __FOODBOX_H
#define __FOODBOX_H
#include <linux/cdev.h>

#define VOLUME	32

struct food {
	char name[64]; // The food name, bone, cookie or meat
	int weight; // how much the food is
	int time; // how long the food is store
	struct food *prev;
	struct food *next;
};

struct foodbox {
	int volume; // how much food the foodbox can store
	struct cdev cdev;
	struct food *food; // here, store the real food
};

#endif
