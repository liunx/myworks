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

	fd = open("/dev/foodbox", O_WRONLY);
	if (fd < 0) {
		perror("Failed to open the device!\n");
		exit(-1);
	}

	food = malloc(sizeof(struct food));
	if (food == NULL)
		exit(-1);

	strcpy(food->name, "cookie");
	food->weight = 50;
	food->time = 0;
	food->prev = NULL;
	food->next = NULL;

	printf("Writting the data...\n");

	len = write(fd, food, sizeof(struct food));
	if (len < sizeof(struct food))
		printf("Write the struct food not complete!\n");
	printf("The length of data written is %d\n", len);

	close(fd);

	return 0;

}
