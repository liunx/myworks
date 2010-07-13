#include <linux/module.h>

static int __init hello_world(void)
{
	printk("Hello, world!\n");
	return 0;
}

static void __exit goodbye_world(void)
{
	printk("goodbye world!\n");
}

module_init(hello_world);
module_exit(goodbye_world);


