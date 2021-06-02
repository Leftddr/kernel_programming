#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
//#include<curses.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

#define DEVICE_NAME	"/dev/kbd_led_drv"

int main(int argc, char* argv[])
{
	int dev;
	char buf[2];
	char a;

	dev = open(DEVICE_NAME, O_RDWR | O_NDELAY);
	
	if(dev < 0)
	{
		printf("fail to device file open\n");
		return 0;
	}

	while(1)
	{
		//preprocessing argv;
		if(argc != 2 || strlen(argv[1]) != 1)
		{
			printf("Input argument(| 0 | 1 | f | + | - |) : ");
			scanf("%s", buf);
			fflush(stdin);
		}
		else
		{
			buf[0] = *argv[1];
			argc = 1;
		}
		buf[1] = '\0';
		int retval = write(dev, buf, 2);
		printf("Do you want to exit ? (Y / N)");
		scanf("%s", buf);
		fflush(stdin);
		if(buf[0] == 'y' || buf[0] == 'Y') return 0;
	}
}
