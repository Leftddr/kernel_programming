#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int sim_fd;
	int x, y;
	char buffer[10];

	sim_fd = open("/dev/input/mouse0", O_RDWR);
	if(sim_fd < 0){
		perror("Could't open mouse0 file\n");
		exit(-1);
	}

	while(1){
		sleep(1);
	}

	close(sim_fd);
}
