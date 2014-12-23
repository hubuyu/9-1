#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/input.h>

#define KEY_DEVICE	"/dev/input/event0"

int main(int argc, char **argv)
{
    int fd;
	int err ;
	int cnt = 0;
	struct input_event ev_data ;

    fd = open( KEY_DEVICE , O_RDONLY);
    if(fd < 0)
    {
         printf("cannot open device-%d \n" ,KEY_DEVICE );
         exit(1);
    }

	printf("Open device successful\n");

    do{
        err = read( fd, &ev_data , sizeof(struct input_event));
		if(err < 0)
		{
			perror("Read event error \n");
			exit(1);
		}
		printf("Loop-%d \n" , cnt);
		if( EV_KEY == ev_data.type )
		{
			printf("	Key type = %d , Key code = %d , Key Value = %d \n ",
					ev_data.type , ev_data.code , ev_data.value);
		}
    }while(cnt++<=60);

    close(fd);
    return 0;
}

