#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

//global thread variables
static struct task_struct *kthread_1;
static struct task_struct *kthread_2;
static int t1 = 1;
static int t2 = 2;

int thread_function(void *thread_num) {
	unsigned int i = 0;

	int t_num = *(int*) thread_num;

	while(!kthread_should_stop()) {
		printk("igor_kthread - Thread %d is executing! Counter value is %d\n", t_num, i);

		i++;
		
		msleep(t_num * 1000);
	}

	printk("igor_kthread - Thread %d finished execution!\n", t_num);

	return 0;
}

/* Meta Information */
MODULE_LICENSE("GPL");

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init my_init(void) {
	printk("igor_kthread - Hello, Kernel!\n");

	kthread_1 = kthread_create(thread_function, &t1, "igor_kthread_1");

	if(kthread_1 != NULL) {
		wake_up_process(kthread_1);

		printk("igor_kthread - Thread 1 was created and is running now!\n");
	}
	else {
		printk("igor_kthread - Thread 1 could not be created!\n");
		return -1;
	}

	kthread_2 = kthread_run(thread_function, &t2, "igor_kthread_2");

	if(kthread_2 == NULL) {
		printk("igor_kthread - Thread 2 could not be created!\n");

		kthread_stop(kthread_1);

		return -1;
	}

	printk("igor_kthread - Both thread 1 and 2 are running now!\n");

	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit my_exit(void) {
	printk("igor_kthread - Goodbye, Kernel\n");

	kthread_stop(kthread_1);
	kthread_stop(kthread_2);
}

module_init(my_init);
module_exit(my_exit);