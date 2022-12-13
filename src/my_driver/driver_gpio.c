#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/device.h>
#include <linux/cdev.h>

// GPIO 硬件相关宏定义
#define MYGPIO_HW_ENABLE

// 设备名称
#define MYGPIO_NAME		"gxt_gpio"

// 一共有４个 GPIO 口
#define MYGPIO_NUMBER		4

// 设备类
static struct class *gpio_class;

// 用来保存设备
struct cdev gpio_cdev[MYGPIO_NUMBER];

// 用来保存设备号
int gpio_major = 0;
int gpio_minor = 0;

#ifdef MYGPIO_HW_ENABLE
// 硬件初始化函数，在驱动程序被加载的时候(gpio_driver_init)被调用
static void gpio_hw_init(int gpio)
{
	printk("gpio_hw_init is called: %d. \n", gpio);
}

// 硬件释放
static void gpio_hw_release(int gpio)
{
	printk("gpio_hw_release is called: %d. \n", gpio);
}

// 设置硬件GPIO的状态，在控制GPIO的时候(gpio_ioctl)被调用
static void gpio_hw_set(unsigned long gpio_no, unsigned int val)
{
	printk("gpio_hw_set is called. gpio_no = %ld, val = %d. \n", gpio_no, val);
}
#endif

// 当应用程序打开设备的时候被调用
static int gpio_open(struct inode *inode, struct file *file)
{
	
	printk("gpio_open is called. \n");
	return 0;	
}

// 当应用程序控制GPIO的时候被调用
static long gpio_ioctl(struct file* file, unsigned int val, unsigned long gpio_no)
{
	printk("gpio_ioctl is called. \n");
	
	// 检查设置的状态值是否合法
	if (0 != val && 1 != val)
	{
		printk("val is NOT valid! \n");
		return 0;
	}

    // 检查设备范围是否合法
	if (gpio_no >= MYGPIO_NUMBER)
	{
		printk("dev_no is invalid! \n");
		return 0;
	}

	printk("set GPIO: %ld to %d. \n", gpio_no, val);

#ifdef MYGPIO_HW_ENABLE
    // 操作 GPIO 硬件
	gpio_hw_set(gpio_no, val);
#endif

	return 0;
}

static const struct file_operations gpio_ops=
{
	.owner = THIS_MODULE,
	.open  = gpio_open,
	.unlocked_ioctl = gpio_ioctl
};

static int __init gpio_driver_init(void)
{
	int i, devno;
	dev_t num_dev;

	printk("gpio_driver_init is called. \n");

	// 动态申请设备号(严谨点的话，应该检查函数返回值)
	alloc_chrdev_region(&num_dev, gpio_minor, MYGPIO_NUMBER, MYGPIO_NAME);

	// 获取主设备号
	gpio_major = MAJOR(num_dev);
	printk("gpio_major = %d. \n", gpio_major);
	// printk("gpio_minor = %d. \n", gpio_minor);

	// 创建设备类
	gpio_class = class_create(THIS_MODULE, MYGPIO_NAME);

	// 创建设备节点
	for (i = 0; i < MYGPIO_NUMBER; ++i)
	{
		// 设备号
		devno = MKDEV(gpio_major, gpio_minor + i);
		
		// 初始化 cdev 结构
		cdev_init(&gpio_cdev[i], &gpio_ops);

		// 注册字符设备
		cdev_add(&gpio_cdev[i], devno, 1);

		// 创建设备节点
		device_create(gpio_class, NULL, devno, NULL, MYGPIO_NAME"%d", i);
	}

#ifdef MYGPIO_HW_ENABLE
    // 初始化 GPIO 硬件
	for (i = 0; i < MYGPIO_NUMBER; ++i)
	{
		gpio_hw_init(i);
	}
#endif

	return 0;
}

static void __exit gpio_driver_exit(void)
{
	int i;
	printk("gpio_driver_exit is called. \n");

	// 删除设备和设备节点
	for (i = 0; i < MYGPIO_NUMBER; ++i)
	{
		cdev_del(&gpio_cdev[i]);
		device_destroy(gpio_class, MKDEV(gpio_major, gpio_minor + i));
	}

	// 释放设备类
	class_destroy(gpio_class);

#ifdef MYGPIO_HW_ENABLE
    // 释放 GPIO 硬件
	for (i = 0; i < MYGPIO_NUMBER; ++i)
	{
		gpio_hw_release(i);
	}
#endif

	// 注销设备号
	unregister_chrdev_region(MKDEV(gpio_major, gpio_minor), MYGPIO_NUMBER);
}

MODULE_LICENSE("GPL");
module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

