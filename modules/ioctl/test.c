#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "ioctl.h"

int main()
{
	int fd;

	fd = open("/dev/ioctl0", O_RDWR);
	if (fd < 0)
		printf("Fail to open the device!\n");
	ioctl(fd, IOCTL_HELLO);
	ioctl(fd, IOCTL_READ, 0);
	ioctl(fd, IOCTL_WRITE, 0);
	ioctl(fd, IOCTL_WR, 0);

	return 0;
}
