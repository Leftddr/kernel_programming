/* **************** LDD:2.0 s_20/lab2_one_tasklet.c **************** */
/*
 * The code herein is: Copyright Jerry Cooperstein, 2012
 *
 * This Copyright is retained for the purpose of protecting free
 * redistribution of source.
 *
 *     URL:    http://www.coopj.com
 *     email:  coop@coopj.com
 *
 * The primary maintainer for this code is Jerry Cooperstein
 * The CONTRIBUTORS file (distributed with this
 * file) lists those known to have contributed to the source.
 *
 * This code is distributed under Version 2 of the GNU General Public
 * License, which you should have received with the source.
 *
 */
/*
 * Shared Interrupts and Bottom Halves (tasklet solution)
 *
 * Write a module that shares its IRQ with your network card.  You can
 * generate some network interrupts either by browsing or pinging.
 *
 * Make it use a top half and a bottom half.
 *
 * Check /proc/interrupts while it is loaded.
 *
 * Have the module keep track of the number of times the interrupt's
 * halves are called.
 *
 * Implement the bottom half using:
 *
 * tasklets.
 *
 * work queues
 *
 * A background thread which you launch during the module's
 * initialization, which gets woken up anytime data is available.
 * Make sure you kill the thread when you unload the module, or it may
 * stay in a zombie state forever.
 *
 * For any method you use does, are the bottom and top halves called
 * an equal number of times?  If not why, and what can you do about
 * it?
 *
 * Note: the solutions introduce a delay parameter which can be set
 * when loading the module; this will introduce a delay of that many
 * milliseconds in the top half, which will provoke dropping even more
 * bottom halves, depending on the method used.x
 *
@*/

#include <linux/module.h>
#include "lab_one_interrupt.h"
#define MYIOC_TYPE 'k'

static struct mydata{
	unsigned long int maxjiffies, minjiffies, aver_jiffies;
}mydata;
/*
static void t_fun(unsigned long t_arg)
{
	struct my_dat *data = (struct my_dat *)t_arg;
	atomic_inc(&counter_bh);
	pr_info(
	       "In BH: counter_th = %d, counter_bh = %d, jiffies=%ld, %ld\n",
	       atomic_read(&counter_th), atomic_read(&counter_bh),
	       data->jiffies, jiffies);
}

static DECLARE_TASKLET(t_name, t_fun, (unsigned long)&my_data);
*/
/* initialize tasklet */
static irqreturn_t my_interrupt(int irq, void *dev_id)
{
	//struct my_dat *data = (struct my_dat *)dev_id;
	if(START == 1){
		if(prevjiffies == -1) prevjiffies = jiffies;
		else {
			unsigned long interval = jiffies - prevjiffies;
			if(interval < minjiffies) minjiffies = interval;
			if(interval > maxjiffies) maxjiffies = interval;
			forjiffies += interval;
		}
		N++;
		pr_info("N = %u, MAX_jiffies = %ld, MIN_jiffies = %ld, Aver_jiffies = %ld\n", N, maxjiffies, minjiffies, (forjiffies / N));
		if(N == 1000){
			START = 0; N = 0; prevjiffies = -1;
			minjiffies = 1e12; maxjiffies = 0; forjiffies = 0;
		}
	}
	pr_info("Interrupt Occured\n"); 
	//atomic_inc(&counter_th);
//	data->jiffies = jiffies;
//	tasklet_schedule(&t_name);
	mdelay(delay);		/* hoke up a delay to try to cause pileup */
	return IRQ_NONE;	/* we return IRQ_NONE because we are just observing */
}

static inline long
mycdrv_unlocked_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	int size, rc, direction;
	void __user *ioargp = (void __user *)arg;
		
	if(_IOC_TYPE(cmd) != MYIOC_TYPE){
		pr_info(" got invalid case, CMD = %d\n", cmd);
		return -EINVAL;
	}
	
	direction = _IOC_DIR(cmd);
	size = _IOC_SIZE(cmd);
	
	switch (direction) {
	case _IOC_READ:
		if(size <= 8)
			rc = copy_to_user(ioargp, &START, size);
		else{
			mydata.maxjiffies = maxjiffies; mydata.minjiffies = minjiffies;
			mydata.aver_jiffies = (forjiffies / N);
			rc = copy_to_user(ioargp, &mydata, size);
		}
		return rc;
		break;
	case _IOC_WRITE:
		rc = copy_from_user(&START, ioargp, size);
		pr_info("START = %u\n", START);
		return rc;
		break;
	default:
		pr_info(" got invalid case, CMD=%d\n", cmd);
		return -EINVAL;
	}
	return 0;
}

static const struct file_operations mycdrv_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = mycdrv_unlocked_ioctl,
	.open = mycdrv_generic_open,
	.release = mycdrv_generic_release
};

module_init(my_generic_init);
module_exit(my_generic_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LDD:2.0 s_20/lab2_one_tasklet.c");
MODULE_LICENSE("GPL v2");
