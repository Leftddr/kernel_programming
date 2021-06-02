#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

struct input_dev *vms_input_dev;
static struct platform_device *vms_dev;

static ssize_t
write_vms(struct device *dev, struct device_attribute *attr, const char *buffer, size_t count)
{
	int x, y;
	sscanf(buffer, "%d%d", &x, &y);
	input_report_rel(vms_input_dev, REL_X, x);	// REL_X의 상태를 커널에 보고
	input_report_rel(vms_input_dev, REL_Y, y);	// REL_Y의 상태를 커널에 보고
	input_sync(vms_input_dev);		// 복수 개의 입력이 하나의 이벤트로 발생하였다는 것을 커널에 알린
	printk(KERN_INFO"write_vms called !!! x = %d, y = %d\n", x, y);
	return count;
}

// sysfs - 유저 영역에서 커널과의 통신을 위해 쓰이는 파일 시스템의 한 종류. 실제 파일 시스템이 아닌 in-memory 파일 시스템이다
// show와 store 함수를 제공하여 커널 내부의 정보를 userspace에 노출 시키거나 userspace에서 access하는 sysfs의 attribute와
// 매핑되는 커널 내부의 자료구조를 변경 함으로써 커널의 동작을 설정할 목적으로 쓰인다
DEVICE_ATTR(coordinates, 0644, NULL, write_vms);	// sysfs의 통신을 위한 파일을 생성
													// _name : sysfs의 파일 이름
													// _mode : 파일의 권한을 준다
													// _show : show함수를 등록
													// _store : store 함수를 등록다

static struct attribute *vms_attrs[] = {		// vms의 kobj에 등록할 attribute들의 배열 초기화
	&dev_attr_coordinates.attr,
	NULL
};

static struct attribute_group vms_attr_group = {	// attribute_group으로 등록
	.attrs = vms_attrs,
};

int __init vms_init(void)
{
	//platform_device 구조체를 할당한다.
	vms_dev = platform_device_register_simple("vms", -1, NULL, 0);	// platform_device를 등록; /sys/devices/platform/ 에 vms라는 디렉토리가 생성된다
	if(IS_ERR(vms_dev)){
		PTR_ERR(vms_dev);
		printk("vms_init error\n");
		return PTR_ERR(vms_dev);
	}

	sysfs_create_group(&vms_dev->dev.kobj, &vms_attr_group);	// vms의 kobj에 vms_attr_group을 등록; vms 디렉토리 밑에 coordinates라는 파일이 생성된다

	printk("Before input_allocate_device()\n");
	mdelay(5000);
	vms_input_dev = input_allocate_device();		// 커널영역의 메모리를 할당하여 input_dev를 만들고 이를 반환
	printk("After input_allocate_device()\n");
	mdelay(5000);
	if(!vms_input_dev){
		printk("Bad input_allocate_device()\n");return -ENOMEM;
	}

	set_bit(EV_REL, vms_input_dev->evbit);		// 이벤트타입을 EV_REL(상대적 좌표)로 설정
	set_bit(REL_X, vms_input_dev->relbit);		// 핸들링할 이벤트를 설정
	set_bit(REL_Y, vms_input_dev->relbit);		//

	printk("Before input_register_device()\n");
	mdelay(5000);
	input_register_device(vms_input_dev);		// 설정을 마친 input_dev를 커널에 등록
	printk("Virtual Mouse Driver Initialized.\n");
	return 0;
}

void vms_cleanup(void)
{
	input_unregister_device(vms_input_dev);

	sysfs_remove_group(&vms_dev->dev.kobj, &vms_attr_group);

	platform_device_unregister(vms_dev);
	return;
}

module_init(vms_init);
module_exit(vms_cleanup);

MODULE_LICENSE("GPL v2");
