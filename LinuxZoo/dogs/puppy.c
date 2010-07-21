/*
 * puppy -- a little dog that is lazy and like food
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FOODBOX		"/dev/foodbox"
#define POWER		16

static int fd;
static int power = POWER;
static enum state {FULL, HUNGRY, FIND, PLAY, EAT} st = FULL;


static void full(void)
{
	if (power >= POWER) {
		printf("Hn, I am full, i want to play games...\n");
		st = PLAY;
		return;
	}
	st = EAT;
	
}

static void hungry(void)
{
	if (power < 0) {
		printf("I am hungry to die!\n");
		exit(-1);
	}
	st = FIND;
}

static void find(void)
{
	fd = open(FOODBOX, O_RDONLY);
	if (fd > 0) {
		printf("Find food!\n");
		st = EAT;
		close(fd);
		return;
	}
	printf("%d\n", power--);
	st = HUNGRY;
}

static void eat(void)
{
	printf("Eating food!\n");
	printf("%d\n", power++);
	st = FULL;
}

static void play(void)
{
	printf("Playing games...\n");
	printf("%d\n", power--);
	st = HUNGRY;
}

int main()
{
	while (1) {

		switch (st) {
			// FULL: the dog is full, then he want play game
			case FULL: 
				full();
				continue;
			// HUNGRY: It's time to find food!
			case HUNGRY:
				hungry();
				continue;
			case FIND:
				find();
				continue;
			case EAT:
				eat();
				continue;
			case PLAY:
				play();
				continue;
			default:
				break;
		}
	}

	return 0;
}


