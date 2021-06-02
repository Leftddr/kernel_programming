#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int sim_fd;
	int x, y;
	char buffer[10];

	sim_fd = open("/sys/devices/platform/vms/coordinates", O_RDWR);
	if(sim_fd < 0){
		perror("Could't open vms coordinate file\n");
		exit(-1);
	}

	while(1){
		x = random()%20;
		y = random()%20;
		if(x%2) 
			x = -x;
		if(y%2)
			y = -y;

		sprintf(buffer, "%d %d %d", x, y, 0);
		printf("%s\n", buffer);

		write(sim_fd, buffer, strlen(buffer));
		fsync(sim_fd);
		sleep(1);
	}

	close(sim_fd);
}
