/* **************** LDD:2.0 s_13/lab1_ioctl_data_test.c **************** */
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
 * Using ioctl's to pass data (User-space application)
 @*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
struct data{
	unsigned long bmaxjiffies;
	unsigned long bminjiffies;
	unsigned long baverjiffies;
	unsigned long tmaxjiffies;
	unsigned long tminjiffies;
	unsigned long taverjiffies;
}data;

int START = 1;
#define MYIOC_TYPE 'k'

int main(int argc, char *argv[])
{
	int fd, rc;
	int MY_IOCTL;
	char *nodename = "/dev/mycdrv";
	char ch;

	/* open the device node */
	if (argc > 1)
		nodename = argv[1];
	fd = open(nodename, O_RDWR);
	printf(" I opened the device node, file descriptor = %d\n", fd);

	/* retrieve the original values; */
	printf("Do you start check network latency? [Y|N]\n");
	scanf("%c", &ch);
	if(ch == 'Y'){
		MY_IOCTL = (int)_IOW(MYIOC_TYPE, 1, sizeof(START));
		rc = ioctl(fd, MY_IOCTL, &START);
		MY_IOCTL = (int)_IOR(MYIOC_TYPE, 1, sizeof(START));
		rc = ioctl(fd, MY_IOCTL, &START);
		while(START){
			rc = ioctl(fd, MY_IOCTL, &START);
			printf("START = %d\n", START);
			sleep(1);
			if(!START) break;
		}
		MY_IOCTL = (int)_IOR(MYIOC_TYPE, 1, struct data);
		rc = ioctl(fd, MY_IOCTL, &data);
		printf("TOP : max_jiffies = %ld, min_jiffies = %ld, aver_jiffies = %ld\n", data.tmaxjiffies, data.tminjiffies, data.taverjiffies); 
		printf("BOT : max_jiffies = %ld, min_jiffies = %ld, aver_jiffies = %ld\n", data.bmaxjiffies, data.bminjiffies, data.baverjiffies); 
		printf("Do you want to Stop? [Y|N]");
		scanf(" %c", &ch);
		if(ch == 'Y') {return 0;}
	}
	
	close(fd);
	exit(0);
}
