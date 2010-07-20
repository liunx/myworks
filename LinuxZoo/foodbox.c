/*
 * foodbox: Every animal need food like meat, vegetable, water etc.
 * 	    I write this foodbox for feeding animals, and as a master,
 * 	    i should add food daily, or animals will die of hungry.
 * author:  Liunx<liunx163@163.com>
 *
 * license: GPL
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h> // file operations
#include <linux/cdev.h> // char device operations
#include <linux/kdev_t.h> // allocate a device number
#include <linux/slab.h> // copy_to/from_user
#include <asm/uaccess.h>
#include <linux/proc_fs.h> // we get info from proc fs
#include <linux/seq_file.h>

#define DEV_FOODBOX		"foodbox"























static int __init foodbox_init(void)
{
	return 0;
}

static void __exit foodbox_exit(void)
{

}

module_init(foodbox_init);
module_exit(foodbox_exit);

MODULE_AUTHOR("Liunx<liunx163@163.com>");
MODULE_LICENSE("GPL");
