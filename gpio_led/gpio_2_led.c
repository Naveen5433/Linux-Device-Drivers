#include<linux/kernel.h>
#include<linux/uaccess.h>
#include<linux/init.h>
#include<linux/gpio.h>
#include<linux/module.h>
#include<linux/fs.h>

#define GPIO_PIN1 529
#define GPIO_PIN2 530

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Naveen");
MODULE_DESCRIPTION("A Simple GPIO LED Driver");

static int gpio_open(struct inode *inode,struct file* file)
{
	printk(KERN_INFO "GPIO Device opened");
	return 0;
}

static int gpio_close(struct inode*inode,struct file *file)
{
	printk(KERN_INFO"GPIO Device closed");
	return 0;
}

static ssize_t gpio_read(struct file *file,char __user *buf,size_t len,loff_t *offset)
{
	int value=gpio_get_value(GPIO_PIN1);
	char buffer[2];

	if(copy_to_user(buf,buffer,2))
	{
		printk(KERN_INFO "Failed to sent data to user\n");
		return -EFAULT;
	}

	pr_info("GPIO read device %d\n",value);
	return 2;
}
static ssize_t gpio_write(struct file* file,const char __user *buf,size_t len,loff_t *offset)
{
	char buffer[4];
	char s1[2],s2[2];
	int op,value;
	if(copy_from_user(buffer,buf,len))
	{
		pr_info("Failed to receive data from user\n");
		return -EFAULT;
	}
	buffer[len]='\0';
	s1[0]=buffer[0];
	s1[1]='\0';
	s2[0]=buffer[2];
	s2[1]='\0';

	op=simple_strtol(s1,NULL,10);
//	op=buffer[0]-48;
	pr_info("op=%d\n",op);
	if(op==1)
	{
	       value=simple_strtol(s2,NULL,10);
              //  value=buffer[2]-48;
	       pr_info("value=%d\n",value);
		if(value==0 || value==1)
		{
			gpio_set_value(GPIO_PIN1,value);
			pr_info("GPIO device write value=%d on pin- %d\n",value,GPIO_PIN1);
		}
		else
		{
			pr_info("Invalid data: GPIO accepts 1 or 0\n");
			return -EINVAL;
		}
	}
	else if(op==2)
	{
		value=simple_strtol(s2,NULL,10);
                pr_info("value=%d\n",value);
		if(value==0 || value==1)
		{
			gpio_set_value(GPIO_PIN2,value);
			pr_info("GPIO device write value=%d on pin- %d\n",value,GPIO_PIN2);
		}
		else
		{
			pr_info("Invalid data: GPIO accepts 1 or 0\n");
			return -EINVAL;
		}

	}
	else
		pr_info("Unknown option\n");


	return len;
}

static struct file_operations gpio_fops={
	.owner=THIS_MODULE,
	.open=gpio_open,
	.release=gpio_close,
	.read=gpio_read,
	.write=gpio_write,
};



static int __init gpio_driver_init(void)
{
	int ret;

  // Request GPIO pin
    ret = gpio_request(GPIO_PIN1, "GPIO_DRIVER");
    if (ret) {
        pr_err("Failed to request GPIO pin %d\n", GPIO_PIN1);
        return ret;
    }

    // Set GPIO direction to output
    ret = gpio_direction_output(GPIO_PIN1, 0);
    if (ret) {
        pr_err("Failed to set GPIO direction for pin %d\n", GPIO_PIN1);
        gpio_free(GPIO_PIN1);
        return ret;
    }
     ret = gpio_request(GPIO_PIN2, "GPIO_DRIVER");
    if (ret) {
        pr_err("Failed to request GPIO pin %d\n", GPIO_PIN2);
        return ret;
    }

    // Set GPIO direction to output
    ret = gpio_direction_output(GPIO_PIN2, 0);
    if (ret) {
        pr_err("Failed to set GPIO direction for pin %d\n", GPIO_PIN2);
        gpio_free(GPIO_PIN2);
        return ret;
    }

    // Register device
    ret = register_chrdev(0, "gpio_driver", &gpio_fops);
    if (ret < 0) {
        pr_err("Failed to register character device\n");
        gpio_free(GPIO_PIN1);
	gpio_free(GPIO_PIN2);
        return ret;
    }

    pr_info("GPIO Driver loaded with %d\n",ret);
    return 0;

}

static void __exit gpio_driver_exit(void)
{
	unregister_chrdev(0,"gpio_driver");
	gpio_free(GPIO_PIN1);
	gpio_free(GPIO_PIN2);
	pr_info("GPIO Driver unloaded\n");
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

