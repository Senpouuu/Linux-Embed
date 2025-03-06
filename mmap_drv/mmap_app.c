/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-3-6 19:47:09
 * @description    :  mmap app
 *------------------------------------------------------------------------**/
#include "fcntl.h"
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static char buf[1024*8];

int main()
{
    int ret;
    int fd = open("/dev/m_mmap", O_RDWR);
    char* mptr;
    if (fd < 0) 
    {
        perror("open");
        exit(1);
    }
    printf("File descriptor: %d\n", fd);
    mptr = (char*)mmap(NULL, 1024*8, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (mptr == MAP_FAILED) 
    {
        perror("mmap");
        exit(1);
    }
    
    strcpy(mptr, "Hello World!");
    ret = read(fd, buf, 1024*8);

    printf("Read %d bytes: %s\n", ret, buf);


    close(fd);
    munmap(mptr, 1024*8); 

    exit(0);


}