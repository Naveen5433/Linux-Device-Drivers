#include<linux/module.h>
#include<linux/kernel.h>

// module initialization function
static int __init basic_driver_init(void)
{
	printk(KERN_INFO"Basic driver loaded\n");
	return 0; // return 0 indicates successful loaded
}

// module cleanup function
static void __exit basic_driver_exit(void)
{
	printk(KERN_INFO"Basic driver unloaded\n");
}

// Register module initialization and cleanup function
module_init(basic_driver_init);
module_exit(basic_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Naveen");
MODULE_DESCRIPTION("A basic linux device driver");
