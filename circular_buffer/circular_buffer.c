#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/uaccess.h> // For copy_to_user and copy_from_user
#include<linux/gfp.h>
#include<linux/slab.h>
#include<asm/semaphore.h>
#define DEVICE_NAME "simple_device"
#define BUFFER_SIZE 1024

static int major_number;
static char device_buffer[BUFFER_SIZE];
static int open_count = 0;

struct circular_buff
{
	char* s;
	struct circular_buff* next;
};

   static struct circular_buff *head=0;
   struct circular_buff *new,*temp;
   static int index;

// Function prototypes for device operations
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);

// File operations structure
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

    if (*offset >= len) // Check if all data has been read
        return 0;

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;
    
    int i,j,z=0;
    temp=head;
    for(i=0;temp;i++)
    {
	    for(j=0;temp->s[j];j++)
		    device_buffer[z++]=temp->s[j];
	     
	     device_buffer[z++]=',';
	     temp=temp->next;

    }
    device_buffer[z]='\0';
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
    
    device_buffer[size] = '\0'; // Null-terminate the string

   index++;
   if(index<=5)
   {
   new=kmalloc(sizeof(struct circular_buff),GFP_KERNEL);
   new->s=kmalloc(strlen(device_buffer)+1,GFP_KERNEL);
   strcpy(new->s,device_buffer);
   new->next=0;
   if(head==0)
   {
	   head=new;
   }
   else
   {
	   temp=head;
	   while(temp->next)
		   temp=temp->next;

           temp->next=new;		   
   }
   
   }
    printk(KERN_INFO "for checking purpose\n");
    int i;
    temp=head;
    for(i=0;temp;i++)
    {
           printk(KERN_INFO "index =%d %s\n",i,temp->s);
           temp=temp->next;
   }	    
    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", size);
   
    printk(KERN_INFO "simple_device: new size =%zu bytes from the user\n", size);
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
