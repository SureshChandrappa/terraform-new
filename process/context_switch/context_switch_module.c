#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

// Function to simulate a task
static int task_function(void *data) {
    while (!kthread_should_stop()) {
        printk(KERN_INFO "Task running: %s\n", (char *)data);
        ssleep(1); // Sleep for 1 second
    }
    return 0;
}

static struct task_struct *task1;
static struct task_struct *task2;

static int __init my_module_init(void) {
    printk(KERN_INFO "Kernel Module Loaded\n");

    // Create two kernel threads
    task1 = kthread_run(task_function, "Task 1", "task1_thread");
    if (IS_ERR(task1)) {
        printk(KERN_ERR "Failed to create task1\n");
        return PTR_ERR(task1);
    }

    task2 = kthread_run(task_function, "Task 2", "task2_thread");
    if (IS_ERR(task2)) {
        printk(KERN_ERR "Failed to create task2\n");
        kthread_stop(task1);
        return PTR_ERR(task2);
    }

    return 0;
}

static void __exit my_module_exit(void) {
    printk(KERN_INFO "Kernel Module Unloaded\n");
    if (task1) {
        kthread_stop(task1);
    }
    if (task2) {
        kthread_stop(task2);
    }
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XXXXXX");
MODULE_DESCRIPTION("A simple kernel module demonstrating context switching");

