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

#include "foodbox.h"

#define DEV_FOODBOX		"foodbox"
#define FBOX_AMOUNT		1

static unsigned int foodbox_major = 0;
static unsigned int foodbox_minor = 0;

static dev_t dev;
static struct foodbox *fbox;


/*
 * eat food -- the animals will do this
 */
static ssize_t
fbox_eat(struct file *filp, char __user *user, size_t len, loff_t *offset)
{

	return 0;

}

static void
add_food(struct food *fd)
{

	if (fbox->food == NULL)
		fbox->food = fd; // We add the first food
}
	

/*
 * add food -- it's the duty of master
 * and the master can put many kinds food
 * like bone, cookie, meat etc.
 */
static ssize_t
fbox_add(struct file *filp, const char __user *user, size_t len, loff_t *offset)
{
	int retval = -ENOMEM;
	int fsize = sizeof(struct food);
	// We get a complete struct food
	struct food *food = kmalloc(fsize, GFP_KERNEL);
	if (!food)
		return -ENOMEM;
	// OK, if we get the space, we use it to store 
	// the data from user space
	if (len > fsize)
		len = fsize;

	if (copy_from_user(food, user, len)) {
		printk(KERN_ALERT "Failed copy from user space!\n");
		retval = -EFAULT;
		goto out;
	}
	// if everything is ok
	printk(KERN_ALERT "food->name = %s, food->weight = %d, food->time = %d\n", 
	       food->name, food->weight, food->time);
	fbox->food = food;

	*offset += len;
	retval = len;
out:

	return retval;
}

/*
 * In fact, we do nothing in open function
 */
static int
fbox_open(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "The value of fbox->foods = %d\n", fbox->volume);
	return 0;
}

static int
fbox_close(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations fbox_fops = {
	.owner		= THIS_MODULE,
	.read		= fbox_eat,
	.write		= fbox_add,
	.open		= fbox_open,
	.release	= fbox_close,
};

static void __exit foodbox_exit(void)
{
	kfree(fbox);
	unregister_chrdev_region(dev, FBOX_AMOUNT);

}

static int __init foodbox_init(void)
{
	int ret;

	dev = MKDEV(foodbox_major, foodbox_minor);
	if (foodbox_major) {
		ret = register_chrdev_region(dev, FBOX_AMOUNT, DEV_FOODBOX);
		if (ret < 0)
			goto cleanup;
	}
	
	ret = alloc_chrdev_region(&dev, foodbox_minor, FBOX_AMOUNT, DEV_FOODBOX);
	if (ret < 0)
		goto cleanup;

	// Now, we can allocate space for our foodbox
	fbox = kmalloc(sizeof(struct foodbox), GFP_KERNEL);
	if (!fbox) {
		ret = -ENOMEM;
		goto cleanup;
	}

	// For safe, we should fill the room full of zero
	memset(fbox, 0, sizeof(struct foodbox));

	// let's set fbox private members
	fbox->food = NULL;
	cdev_init(&(fbox->cdev), &fbox_fops);
	fbox->cdev.owner = THIS_MODULE;
	fbox->cdev.ops = &fbox_fops;

	ret = cdev_add(&fbox->cdev, dev, 1);
	if (ret)
		goto cleanup;

	return ret;

cleanup:
	foodbox_exit();
	return ret;
}


module_init(foodbox_init);
module_exit(foodbox_exit);

MODULE_AUTHOR("Liunx<liunx163@163.com>");
MODULE_LICENSE("GPL");
