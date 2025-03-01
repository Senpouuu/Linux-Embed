/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-3-1 17:04:07
 * @description    :  block/nblock key driver application   
 *------------------------------------------------------------------------**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/types.h>
#include <poll.h>

int main(int argc, char *argv[])
{
    int fd;
    char key_stat = 0;
    if(argc < 2)
    {
        printf("Usage: %s block/nblock\n", argv[0]);
        exit(0);
    }

    if(strcmp(argv[1], "block") == 0)
        fd = open("/dev/key_drv", O_RDWR);

    else if(strcmp(argv[1], "nblock") == 0)
        fd = open("/dev/key_drv", O_RDWR|O_NONBLOCK); 

    else
    {
        printf("Usage: %s block/nblock\n", argv[0]);
        exit(0);
    }
    
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

    while(1)
    {
        printf("waiting for key input...\n");
        key_stat = read(fd, &key_stat, 1);
        printf("key_stat = %d\n", key_stat);
    }

    close(fd);
    return 0;
}
