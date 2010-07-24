#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "addfood.h"

int main()
{
	int fd, len;
	struct food *food = NULL;

	fd = open("/dev/foodbox", O_RDONLY);
	if (fd < 0) {
		perror("Failed to open the device!\n");
		exit(-1);
	}

	food = malloc(sizeof(struct food));
	if (food == NULL)
		exit(-1);

	printf("Reading the data...\n");

	len = read(fd, food, sizeof(struct food));
	if (len <= 0) {
		printf("There's no food, my god!\n");
		close(fd);
		exit(-1);
	}
	printf("Name: %s, Weight: %d, time: %d\n", food->name,
			food->weight, food->time);
	printf("The length of data read is %d\n", len);

	close(fd);

	return 0;

}
