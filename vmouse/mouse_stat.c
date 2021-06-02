#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#define MOUSEFILE "/dev/input/mouse0"

int main()
{
	int fd;
	struct input_event ie;

	unsigned char btnLeft, btnMiddle, btnRight;

	if((fd= open(MOUSEFILE, O_RDONLY)) == -1){
		perror("Error while opening device\n");
		exit(EXIT_FAILURE);
	} else {
		printf("Device opended\n");
	}

	while(read(fd, &ie, sizeof(struct input_event))){
		unsigned char *data = (unsigned char*)&ie;
		btnLeft =  data[0] & 0x1;
		btnMiddle = !!(data[0] & 0x4);
		btnRight = !!(data[0] & 0x2);
		printf("Left:%d, Middle:%d, Right:%d\n", btnLeft, btnMiddle, btnRight); 
	}
	return 0;
}
