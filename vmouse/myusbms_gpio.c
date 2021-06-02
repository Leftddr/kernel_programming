#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

/* for apple IDs */
#ifdef CONFIG_USB_HID_MODULE
#include "hid-ids.h"
#endif

//gpio-ok04.c
#include <linux/fs.h>
#include <asm/io.h>

#define DEV_UMG_MAJOR_NUMBER	226
#define DEV_UMG_NAME		"myusbms_gpio"

#define CUR_GPIO		16

#define M_INPUT			0
#define M_OUTPUT		1
#define S_LOW			0
#define S_HIGH			1
#define S_OFF			0
#define S_ON			1

// BCM2835 in B+, BCM2836 in 2
#define BCM2836_PERI_BASE	0x3F000000
#define GPIO_BASE	(BCM2836_PERI_BASE + 0x00200000)
//gpio-ok04.c

/*
 * Version Information
 */
#define DRIVER_VERSION "v1.6"
#define DRIVER_AUTHOR "Vojtech Pavlik <vojtech@ucw.cz>"
#define DRIVER_DESC "USB HID Boot Protocol mouse driver"
#define DRIVER_LICENSE "GPL"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE(DRIVER_LICENSE);
static struct timer_list my_timer;

volatile unsigned int *gpio;

unsigned int i = 5;
unsigned int pin = 1;

struct usb_mouse {
	char name[128];
	char phys[64];
	struct usb_device *usbdev;
	struct input_dev *dev;
	struct urb *irq;

	signed char *data;
	dma_addr_t data_dma;
};

static int set_bits(
	volatile unsigned int *addr, 
	const unsigned int shift, 
	const unsigned int val, 
	const unsigned int mask)
{
	unsigned int temp = *addr;
			
	temp &= ~(mask << shift);
	temp |= (val & mask) << shift;
	*addr = temp;

	return 0;
}

static int func_pin(
	const unsigned int pin_num,
	const unsigned int mode)
{
	unsigned int pin_bank = pin_num / 10;

	if(pin_num > 53) return -1;
	if(mode > 7) return -1;

	gpio += pin_bank;

	set_bits(gpio, (pin_num % 10) * 3, mode, 0x7);
	gpio -= pin_bank;

	return 0;
}

static int set_pin(
	const unsigned int pin_num, 
	const unsigned int status)
{
	unsigned int pin_bank = pin_num >> 5;

	if(pin_num > 53) return -1;
	if(status != S_OFF && status != S_ON) return -1;

	gpio += pin_bank;

	if(status == S_OFF){
		set_bits(
			gpio + 0x28/sizeof(unsigned int), 
			pin_num, S_HIGH, 0x1);
		
		set_bits(
			gpio + 0x28/sizeof(unsigned int), 
			pin_num, S_LOW, 0x1);
	}else if(status == S_ON){
		set_bits(
			gpio + 0x1C/sizeof(unsigned int), 
			pin_num, S_HIGH, 0x1);
		set_bits(
			gpio + 0x1C/sizeof(unsigned int), 
			pin_num, S_LOW, 0x1);
	}
	gpio -= pin_bank;

	return 0;
}

static void my_timer_function(struct timer_list * ptr)
{
	pr_info("I am in my_timer_fun, jiffies = %ld\n", jiffies);
	pr_info(" my data is: %d\n", (int)ptr);

	if(pin==1){
		pin = 0;
		i--;
		if(set_pin(CUR_GPIO, S_OFF) != 0){
			printk("[gpio-ok04] set_pin() error!\n");
			return;
		}
	}
	else if(pin==0) {
		pin = 1;
		if(set_pin(CUR_GPIO, S_ON) != 0){
			printk("[gpio-ok04] set_pin() error!\n");
			return;
		}
	}

	if(i > 0){
		pr_info("i : %d\n", i);
		mod_timer(&my_timer, HZ + jiffies);
	}
	else{
		i=5;
		return;
	}
}

static void usb_mouse_irq(struct urb *urb)
{
	struct usb_mouse *mouse = urb->context;
	signed char *data = mouse->data;
	struct input_dev *dev = mouse->dev;
	int status;

	switch (urb->status) {
	case 0:			/* success */
		break;
	case -ECONNRESET:	/* unlink */
	case -ENOENT:
	case -ESHUTDOWN:
		return;
	/* -EPIPE:  should clear the halt */
	default:		/* error */
		goto resubmit;
	}

	input_report_key(dev, BTN_LEFT,   data[0] & 0x01);
	input_report_key(dev, BTN_RIGHT,  data[0] & 0x02);
	input_report_key(dev, BTN_MIDDLE, data[0] & 0x04);
	input_report_key(dev, BTN_SIDE,   data[0] & 0x08);
	input_report_key(dev, BTN_EXTRA,  data[0] & 0x10);

	if(data[0] & 0x1)	{
		//turn on LED
		if(set_pin(CUR_GPIO, S_ON) != 0){
			printk("[usbmouse_gpio] set_pin() error!\n");
			return;
		}
		printk("Clicked Left Button : LED On\n");
	}

	if(data[0] & 0x2)	{
		//turn off LED
		if(set_pin(CUR_GPIO, S_OFF) != 0){
			printk("[usbmouse_gpio] set_pin() error!\n");
			return;
		}
		printk("Clicked Right Button : LED Off\n");
	}

	if(data[0] & 0x4)	{
		//turn LED on and off 10 times
		printk("Clicked Middle Button : LED flashes 5 times\n");
		if(set_pin(CUR_GPIO, S_ON) != 0){
			printk("[gpio-ok03] set_pin() error!\n");
			return;
		}
		mod_timer(&my_timer, jiffies + HZ);		
	}
	input_report_rel(dev, REL_X,     data[1]);
	input_report_rel(dev, REL_Y,     data[2]);
	input_report_rel(dev, REL_WHEEL, data[3]);

	input_sync(dev);
resubmit:
	status = usb_submit_urb (urb, GFP_ATOMIC);
	if (status)
		dev_err(&mouse->usbdev->dev,
			"can't resubmit intr, %s-%s/input0, status %d\n",
			mouse->usbdev->bus->bus_name,
			mouse->usbdev->devpath, status);
}

static int usb_mouse_open(struct input_dev *dev)
{
	struct usb_mouse *mouse = input_get_drvdata(dev);

	mouse->irq->dev = mouse->usbdev;
	if (usb_submit_urb(mouse->irq, GFP_KERNEL))
		return -EIO;

	printk("[usbmouse_gpio] opened\n");
/*
	init_timer(&my_timer);	
	my_timer.function = my_timer_function;   */

	timer_setup(&my_timer, my_timer_function, 0);
	my_timer.expires = jiffies + HZ;	/* one second delay */

	gpio = (volatile unsigned int *)ioremap(GPIO_BASE, 4096);
	
	if(func_pin(CUR_GPIO, M_OUTPUT) != 0){
		printk("[usbmouse_gpio] func_pin() error!\n");
		return -1;
	}
	return 0;
}

static void usb_mouse_close(struct input_dev *dev)
{
	struct usb_mouse *mouse = input_get_drvdata(dev);

	set_pin(CUR_GPIO, S_OFF);

	usb_kill_urb(mouse->irq);

	printk("[usbmouse_gpio] closed\n");
}

static int usb_mouse_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	struct usb_host_interface *interface;
	struct usb_endpoint_descriptor *endpoint;
	struct usb_mouse *mouse;
	struct input_dev *input_dev;
	int pipe, maxp;
	int error = -ENOMEM;

	interface = intf->cur_altsetting;

	if (interface->desc.bNumEndpoints != 1)
		return -ENODEV;

	endpoint = &interface->endpoint[0].desc;
	if (!usb_endpoint_is_int_in(endpoint))
		return -ENODEV;

	pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
	maxp = usb_maxpacket(dev, pipe, usb_pipeout(pipe));

	mouse = kzalloc(sizeof(struct usb_mouse), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!mouse || !input_dev)
		goto fail1;

	mouse->data = usb_alloc_coherent(dev, 8, GFP_ATOMIC, &mouse->data_dma);
	if (!mouse->data)
		goto fail1;

	mouse->irq = usb_alloc_urb(0, GFP_KERNEL);
	if (!mouse->irq)
		goto fail2;

	mouse->usbdev = dev;
	mouse->dev = input_dev;

	if (dev->manufacturer)
		strlcpy(mouse->name, dev->manufacturer, sizeof(mouse->name));

	if (dev->product) {
		if (dev->manufacturer)
			strlcat(mouse->name, " ", sizeof(mouse->name));
		strlcat(mouse->name, dev->product, sizeof(mouse->name));
	}

	if (!strlen(mouse->name))
		snprintf(mouse->name, sizeof(mouse->name),
			 "USB HIDBP Mouse %04x:%04x",
			 le16_to_cpu(dev->descriptor.idVendor),
			 le16_to_cpu(dev->descriptor.idProduct));

	usb_make_path(dev, mouse->phys, sizeof(mouse->phys));
	strlcat(mouse->phys, "/input0", sizeof(mouse->phys));

	input_dev->name = mouse->name;
	input_dev->phys = mouse->phys;
	usb_to_input_id(dev, &input_dev->id);
	input_dev->dev.parent = &intf->dev;

	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) |
		BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
	input_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
	input_dev->keybit[BIT_WORD(BTN_MOUSE)] |= BIT_MASK(BTN_SIDE) |
		BIT_MASK(BTN_EXTRA);
	input_dev->relbit[0] |= BIT_MASK(REL_WHEEL);

	input_set_drvdata(input_dev, mouse);

	input_dev->open = usb_mouse_open;
	input_dev->close = usb_mouse_close;

	usb_fill_int_urb(mouse->irq, dev, pipe, mouse->data,
			 (maxp > 8 ? 8 : maxp),
			 usb_mouse_irq, mouse, endpoint->bInterval);
	mouse->irq->transfer_dma = mouse->data_dma;
	mouse->irq->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

	error = input_register_device(mouse->dev);
	if (error)
		goto fail3;

	usb_set_intfdata(intf, mouse);
	printk("USB mouse probed!\n");
	return 0;

fail3:	
	usb_free_urb(mouse->irq);
fail2:	
	usb_free_coherent(dev, 8, mouse->data, mouse->data_dma);
fail1:	
	input_free_device(input_dev);
	kfree(mouse);
	return error;
}

static void usb_mouse_disconnect(struct usb_interface *intf)
{
	struct usb_mouse *mouse = usb_get_intfdata (intf);

	usb_set_intfdata(intf, NULL);
	if (mouse) {
		usb_kill_urb(mouse->irq);
		input_unregister_device(mouse->dev);
		usb_free_urb(mouse->irq);
		usb_free_coherent(interface_to_usbdev(intf), 8, mouse->data, mouse->data_dma);
		kfree(mouse);
	}
}

static struct usb_device_id usb_mouse_id_table [] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_MOUSE) },
	{ }	/* Terminating entry */
};

MODULE_DEVICE_TABLE (usb, usb_mouse_id_table);

static struct usb_driver usb_mouse_driver = {
	.name		= "myusbms_gpio",
	.probe		= usb_mouse_probe,
	.disconnect	= usb_mouse_disconnect,
	.id_table	= usb_mouse_id_table,
};

static int usb_mouse_init(void)
{
	int retval = usb_register(&usb_mouse_driver);
	printk("myusbms_gpio init\n");
	if(retval == 0)
		printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION ": " DRIVER_DESC "\n");
	return retval;
}

static void usb_mouse_exit(void)
{
	pr_info("Deleted time, rc = %d\n", del_timer_sync(&my_timer));
	usb_deregister(&usb_mouse_driver);
	printk("myusbms_gpio exit\n");
}

module_init(usb_mouse_init);
module_exit(usb_mouse_exit);
