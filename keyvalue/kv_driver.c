#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user

#define DEVICE_NAME "simple_device"
#define BUFFER_SIZE 1024

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;

// Function prototypes for device operations
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);

    struct hashtable
    {
	    int key;
	    int value;
    };
// File operations structure
   static struct hashtable new[10];
   static int index;
static struct file_operations fops = {
    .open = device_open,
    .release = device_close,
    .read = device_read,
    .write = device_write,
};

// Called when the device is opened
static int device_open(struct inode *inode, struct file *file) {
    open_count++;
    printk(KERN_INFO "simple_device: Device opened %d time(s)\n", open_count);
    return 0;
}

// Called when the device is closed
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device: Device closed\n");
    return 0;
}

// Called when data is read from the device
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    size_t len = strlen(device_buffer);

            printk(KERN_INFO "\ninside read function\n");
    if (*offset >= len) // Check if all data has been read
        return 0;
            printk(KERN_INFO "\noffset value\n");

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;

    if (copy_from_user(device_buffer, user_buffer, size)) {
            printk(KERN_INFO "\ncopy from user\n");
        return -EFAULT;
     }   
   
    int temp,read_op;
    int i,mul=1;
	    read_op=device_buffer[0]-48;
		    printk(KERN_INFO "\nindex=%d\n",read_op);
	    temp=new[read_op].key;
	    for(mul=1;temp;temp/=10,mul=mul*10);
	    mul=mul/10;
            temp=new[read_op].key;
	    for(i=0;temp;i++)
	    {
		    device_buffer[i]=temp/mul+48;
		    temp=temp%mul;
		    mul=mul/10;
	    }
	    device_buffer[i]=' ';
	    
	    temp=new[read_op].value;
	    for(mul=1;temp;temp/=10,mul=mul*10);
	    mul=mul/10;
            temp=new[read_op].value;
	    for(i=i+1;temp;i++)
	    {
                    
		    device_buffer[i]=temp/mul+48;
		    temp=temp%mul;
		    mul=mul/10;
	    }
	    device_buffer[i]='\0';     

	/*    int i=0,temp,j,mul=1;
	    device_buffer[i++]=index+48;
            device_buffer[i]='\n';
	    for(j=0;j<=index;j++)
	    {
	    temp=new[j].key;
	    for(mul=1;temp;temp/=10,mul=mul*10);
	    mul=mul/10;
            temp=new[j].key;
	    for(i++;temp;i++)
	    {
		    device_buffer[i]=(temp/mul)+48;
		    temp=temp%mul;
		    mul=mul/10;
	    }
	    device_buffer[i]=' ';
	    
	    temp=new[j].value;
	    for(mul=1;temp;temp/=10,mul=mul*10);
	    mul=mul/10;
            temp=new[j].value;
	    for(i=i+1;temp;i++)
	    {
                    
		    device_buffer[i]=temp/mul+48;
		    temp=temp%mul;
		    mul=mul/10;
	    }
            device_buffer[i]='\n';
	    }
	    device_buffer[++i]='\0';
*/

            printk(KERN_INFO "for my checking purpose\n");
	    for(i=0;i<index;i++)
		    printk(KERN_INFO "key=%d value=%d\n",new[i].key,new[i].value);
	    

    if (copy_to_user(user_buffer, device_buffer + *offset, size)) {
        return -EFAULT;
    }

    *offset += size;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size > BUFFER_SIZE - 1) // Limit size to buffer capacity
        size = BUFFER_SIZE - 1;

    if (copy_from_user(device_buffer, user_buffer, size)) {
        return -EFAULT;
    }
    device_buffer[size] = '\0';// Null-terminate the string
    
    
//    static struct hashtable new[10];
  //  static int index;
    int i,sum=0;
/*    if(size<3)
    {
	    read_op=device_buffer[size-2]-48;
	    temp=new[read_op].key;
	    for(mul=1;temp;temp/=10,mul=mul*10);
	    mul=mul/10;
            temp=new[read_op].key;
	    for(i=0;temp;i++)
	    {
		    device_buffer[i]=temp/mul+48;
		    temp=temp%mul;
		    mul=mul/10;
	    }
	    device_buffer[i]=' ';
	    
	    temp=new[read_op].value;
	    for(mul=1;temp;temp/=10,mul=mul*10);
	    mul=mul/10;
            temp=new[read_op].value;
	    for(i=i+1;temp;i++)
	    {
		    device_buffer[i]=temp/mul+48;
		    temp=temp%mul;
		    mul=mul/10;
	    }
	    device_buffer[i]='\0';
            printk(KERN_INFO "for my checking purpose\n");
	    for(i=0;i<index;i++)
		    printk(KERN_INFO "key=%d value=%d\n",new[i].key,new[i].value);
	    return size;
    } */
    if(size>3)
    {
    for(i=0;device_buffer[i];i++)
    {
	    if(device_buffer[i]==' ')
		    break;
            sum=sum*10+(device_buffer[i]-48);
    }
    
    new[index].key=sum;
    sum=0;
    for(i++;device_buffer[i];i++)
    {
	    if(device_buffer[i]==' ')
		    break;
	    sum=sum*10+(device_buffer[i]-48);
    }
    new[index].value=sum;
    
    }
    index++;
    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", size);
    return size;
}

// Module initialization
static int __init simple_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_device: Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "simple_device: Registered with major number %d\n", major_number);
    return 0;
}

// Module cleanup
static void __exit simple_driver_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "simple_device: Unregistered device\n");
}

module_init(simple_driver_init);
module_exit(simple_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Simple Linux Device Driver");
