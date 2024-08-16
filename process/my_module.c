#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

// Function to print numbers
static int print_numbers(void *data) {
    int i;
    for (i = 1; i <= 10; i++) {
        printk(KERN_INFO "Number: %d\n", i);
        ssleep(1); // Sleep for 1 second
    }
    return 0;
}

// Function to print letters
static int print_letters(void *data) {
    char c;
    for (c = 'A'; c <= 'J'; c++) {
        printk(KERN_INFO "Letter: %c\n", c);
        ssleep(1); // Sleep for 1 second
    }
    return 0;
}

static struct task_struct *thread1;
static struct task_struct *thread2;

static int __init my_module_init(void) {
    printk(KERN_INFO "Kernel Module Loaded\n");

    // Create thread for printing numbers
    thread1 = kthread_run(print_numbers, NULL, "print_numbers_thread");
    if (IS_ERR(thread1)) {
        printk(KERN_ERR "Failed to create thread1\n");
        return PTR_ERR(thread1);
    }

    // Create thread for printing letters
    thread2 = kthread_run(print_letters, NULL, "print_letters_thread");
    if (IS_ERR(thread2)) {
        printk(KERN_ERR "Failed to create thread2\n");
        kthread_stop(thread1);
        return PTR_ERR(thread2);
    }

    return 0;
}

static void __exit my_module_exit(void) {
    printk(KERN_INFO "Kernel Module Unloaded\n");
    if (thread1) {
        kthread_stop(thread1);
    }
    if (thread2) {
        kthread_stop(thread2);
    }
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XXXXX");
MODULE_DESCRIPTION("A simple kernel module with two threads");
