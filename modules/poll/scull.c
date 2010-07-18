/*
 * scull: Simple Character Utility for Loading Localities
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/wait.h>

#define SCULL_COUNT	4

static unsigned int scull_major = 0;
static unsigned int scull_minor = 0;
static dev_t dev;

static struct cdev *scull_dev;

static ssize_t 
scull_read(struct file *filp, char __user *user, size_t len, loff_t *offset)
{
	ssize_t retval = 0;
	char buf[32] = "liunx";
	printk(KERN_ALERT "On scull read...\n");
	len = strlen(buf);
	if (*offset > len)
		goto out;

	if (*offset + len > 32)
		len = 32 - *offset;

	if (copy_to_user(user, buf, len)) {
		printk(KERN_ALERT "复制到用户空间失败\n");
	}
	*offset += len;
	retval = len;

out:
	return retval;
}

static ssize_t
scull_write(struct file *filp, const char __user *user, size_t len, loff_t *offset)
{
	char *buf = kmalloc(4096, GFP_KERNEL);

	printk(KERN_ALERT "On scull write...\n");

	if (copy_from_user(buf, user, 32)) {
		printk(KERN_ALERT "从用户区复制数据失败\n");
	}


	return 32;
}

static int
scull_open(struct inode *inode, struct file *filp)
{
	unsigned int imajor, iminor;
	struct cdev *dev = NULL;
	void *data = NULL;
	data = kmalloc(4096 * sizeof(char), GFP_KERNEL);
	if (data != NULL)
		filp->private_data = data;

	printk(KERN_ALERT "On scull_open\n");

	/*
	 * in inode->i_cdev, it contains the cdev
	 * struct with the imformation we had fill
	 * in, so we get the contents
	 */

	dev = inode->i_cdev;
	if (dev == NULL)
		printk(KERN_ALERT "inode 中没有cdev内容\n");
	/*
	 * If the contents is not null
	 * then we store the contens into
	 * file->private_data
	 */
	filp->private_data = dev;

	imajor = MAJOR(inode->i_rdev);
	iminor = MINOR(inode->i_rdev);
	printk(KERN_ALERT "imajor == %d, iminor == %d\n", imajor, iminor);

	/* Let's get some imformation about inode */
	printk(KERN_ALERT "i_uid:%d, i_gid:%d, i_rdev:%d\n", inode->i_uid,
			inode->i_gid, inode->i_rdev);
	return 0;
}

static int
scull_close(struct inode *inodep, struct file *filp)
{
	/*
	 * In fact, we need to do nothing
	 */
	kfree(filp->private_data);
	return 0;
}

static unsigned int
scull_poll(struct file *filp, struct poll_table_struct *wait)
{
	return 0;
}

static struct file_operations scull_fops = {
	.owner		= THIS_MODULE,
	.read		= scull_read,
	.write		= scull_write,
	.open		= scull_open,
	.release	= scull_close,
	.poll		= scull_poll,

};

static int __init scull_init(void)
{
	int ret;
	printk(KERN_ALERT "初始化模块scull_init.\n");
	dev = MKDEV(scull_major, scull_minor);
	if (scull_major) {
	ret = register_chrdev_region(dev, SCULL_COUNT, "scull");
	if (ret < 0)
		printk(KERN_ALERT "Fail to register new dev\n");
	}

	ret = alloc_chrdev_region(&dev, scull_minor, SCULL_COUNT, "scull");
	if (ret < 0)
		printk(KERN_ALERT "Fail to dynamic allocate a new dev\n");
	scull_dev = cdev_alloc();
	if (scull_dev == NULL)
		printk(KERN_ALERT "分配cdev结构失败\n");

	cdev_init(scull_dev, &scull_fops);
	scull_dev->owner = THIS_MODULE;
	scull_dev->ops = &scull_fops;

	ret = cdev_add(scull_dev, dev, 1);
	if (ret < 0)
		printk(KERN_ALERT "添加失败\n");

	return 0;
}

static void __exit scull_exit(void)
{
	printk(KERN_ALERT "退出模块scull_exit.\n");
	unregister_chrdev_region(dev, SCULL_COUNT);
	cdev_del(scull_dev);
}

module_init(scull_init);
module_exit(scull_exit);

MODULE_LICENSE("GPL");
