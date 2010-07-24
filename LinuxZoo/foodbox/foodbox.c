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
#include <linux/semaphore.h>

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
	int retval = -ENOMEM;
	int fsize = sizeof(struct food);
	struct food *food = NULL;
	struct list_head *ptr = NULL;
	struct list_head *tmp = NULL;
	if (len < fsize)
		len = fsize;
	if (list_empty(&fbox->list)) {
#ifdef DEBUG
		printk(KERN_ALERT "Oh my god, there's no food!\n");
#endif
		retval = 0;
		goto out;
	}
	// Or we add the food in the tail 
#ifdef DEBUG
	printk(KERN_ALERT "list all of  food\n");
#endif
	if (down_interruptible(&fbox->sem))
		return -ERESTARTSYS;
	list_for_each_safe(ptr, tmp, &fbox->list) {
		food = list_entry(ptr, struct food, list);
		if (food == NULL) {
#ifdef DEBUG
			printk(KERN_ALERT "There's no food.\n");
#endif
			retval = 0;
			goto out;
		}
#ifdef DEBUG
		printk(KERN_ALERT "Name: %s, Weight: %d, Time: %d\n",
				food->name, food->weight, food->time);
#endif
		list_del(ptr);
		//kfree(food);
		// We just eat one per time
		fbox->volume--;
		break;
	}

	if (copy_to_user(user, food, fsize)) {
#ifdef DEBUG
		printk(KERN_ALERT "Failed copy to user space!\n");
#endif
		retval = -EFAULT;
		goto out;
	}

	kfree(food);

	*offset = fsize;
	retval = fsize;
out:
	up(&fbox->sem);
	return retval;

}

/*
 * add the food structure to the list
 */
static void
add_food(struct food *fd)
{
	// we add the food in the tail 
#ifdef DEBUG
	printk(KERN_ALERT "Add the food to the tail...\n");
#endif
	list_add_tail(&fd->list, &fbox->list);
	fbox->volume++;
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
	if (down_interruptible(&fbox->sem))
		return -ERESTARTSYS;

	if (copy_from_user(food, user, len)) {
#ifdef DEBUG
		printk(KERN_ALERT "Failed copy from user space!\n");
#endif
		retval = -EFAULT;
		goto out;
	}
	// if everything is ok
#ifdef DEBUG
	printk(KERN_ALERT "food->name = %s, food->weight = %d, food->time = %d\n", 
	       food->name, food->weight, food->time);
#endif
	add_food(food);

	*offset += len;
	retval = len;
out:

	up(&fbox->sem);
	return retval;
}

/*
 * In fact, we do nothing in open function
 */
static int
fbox_open(struct inode *inode, struct file *file)
{
//	printk(KERN_ALERT "The value of fbox->foods = %d\n", fbox->volume);
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
	INIT_LIST_HEAD(&fbox->list);
	fbox->volume = 0;
	init_MUTEX(&fbox->sem);
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
