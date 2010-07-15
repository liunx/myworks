/*
 * ioctl: Simple Character Utility for Loading Localities
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#define IOCTL_COUNT	4

static unsigned int ioctl_major = 0;
static unsigned int ioctl_minor = 0;
static dev_t dev;

static struct cdev *ioctl_dev;

static ssize_t 
ioctl_read(struct file *filp, char __user *user, size_t len, loff_t *offset)
{
	ssize_t retval = 0;
	char buf[32] = "liunx";
	printk(KERN_ALERT "On ioctl read...\n");
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
ioctl_write(struct file *filp, const char __user *user, size_t len, loff_t *offset)
{
	char *buf = kmalloc(4096, GFP_KERNEL);

	printk(KERN_ALERT "On ioctl write...\n");

	if (copy_from_user(buf, user, 32)) {
		printk(KERN_ALERT "从用户区复制数据失败\n");
	}


	return 32;
}

static int
ioctl_open(struct inode *inode, struct file *filp)
{
	unsigned int imajor, iminor;
	struct cdev *dev = NULL;
	void *data = NULL;
	data = kmalloc(4096 * sizeof(char), GFP_KERNEL);
	if (data != NULL)
		filp->private_data = data;

	printk(KERN_ALERT "On ioctl_open\n");

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
ioctl_close(struct inode *inodep, struct file *filp)
{
	/*
	 * In fact, we need to do nothing
	 */
	kfree(filp->private_data);
	return 0;
}

static struct file_operations ioctl_fops = {
	.owner		= THIS_MODULE,
	.read		= ioctl_read,
	.write		= ioctl_write,
	.open		= ioctl_open,
	.release	= ioctl_close,

};

static int __init ioctl_init(void)
{
	int ret;
	printk(KERN_ALERT "初始化模块ioctl_init.\n");
	dev = MKDEV(ioctl_major, ioctl_minor);
	if (ioctl_major) {
	ret = register_chrdev_region(dev, IOCTL_COUNT, "ioctl");
	if (ret < 0)
		printk(KERN_ALERT "Fail to register new dev\n");
	}

	ret = alloc_chrdev_region(&dev, ioctl_minor, IOCTL_COUNT, "ioctl");
	if (ret < 0)
		printk(KERN_ALERT "Fail to dynamic allocate a new dev\n");
	ioctl_dev = cdev_alloc();
	if (ioctl_dev == NULL)
		printk(KERN_ALERT "分配cdev结构失败\n");

	cdev_init(ioctl_dev, &ioctl_fops);
	ioctl_dev->owner = THIS_MODULE;
	ioctl_dev->ops = &ioctl_fops;

	ret = cdev_add(ioctl_dev, dev, 1);
	if (ret < 0)
		printk(KERN_ALERT "添加失败\n");

	return 0;
}

static void __exit ioctl_exit(void)
{
	printk(KERN_ALERT "退出模块ioctl_exit.\n");
	unregister_chrdev_region(dev, IOCTL_COUNT);
	cdev_del(ioctl_dev);
}

module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");
