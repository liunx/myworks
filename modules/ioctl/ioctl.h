#ifndef __IOCTL_H
#define __IOCTL_H
#include <linux/ioctl.h>

#define IOC_MAGIC	'L'

#define IOCTL_HELLO	_IO(IOC_MAGIC, 0)
#define IOCTL_READ	_IOR(IOC_MAGIC, 1, int)
#define IOCTL_WRITE	_IOW(IOC_MAGIC, 2, int)
#define IOCTL_WR	_IOWR(IOC_MAGIC, 3, int)

#define IOC_MAXNR	5



#endif
