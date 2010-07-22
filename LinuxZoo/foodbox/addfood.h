#ifndef __ADDFOOD_H
#define __ADDFOOD_H

struct food {
	char name[64]; // The food name, bone, cookie or meat
	int weight; // how much the food is
	int time; // how long the food is store
	struct food *prev; // in fact, the tow pointer are useless in user
	struct food *next; // space, they just hold room
};

#endif
