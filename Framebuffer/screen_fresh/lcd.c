#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#define THREAD_NUM 5

static pthread_t tid_buf[THREAD_NUM];
static struct fb_var_screeninfo vinfo;
static char *fbp;

void lcd_put_pixel(int x, int y, unsigned int color,struct fb_var_screeninfo var,char* fb_base) 
{ 
    int line_width = var.xres * var.bits_per_pixel / 8; 
    int pixel_width = var.bits_per_pixel / 8; 

    unsigned char *pen_8 = fb_base + y * line_width + x * pixel_width; 
    unsigned short *pen_16; 
    unsigned int *pen_32; 

    unsigned int red, green, blue; 

    pen_16 = (unsigned short *)pen_8; 
    pen_32 = (unsigned int *)pen_8; 

    // Extract RGB components
    red   = (color >> 16) & 0xff; 
    green = (color >> 8) & 0xff; 
    blue  = (color >> 0) & 0xff; 

    switch (var.bits_per_pixel) 
    { 
        case 8: 
            *pen_8 = color; 
            break; 
        case 16: 
            // Convert RGB888 to RGB565
            color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3); 
            *pen_16 = color; 
            break; 
        case 32: 
            *pen_32 = color; 
            break; 
        default: 
            printf("can't support %dbpp\n", var.bits_per_pixel); 
            break; 
    } 
}

void* lcd_put_pixel_thr(void *arg)
{
    int step = *(int*)arg;    
    printf("Thread %d start\n",step);
    for(int i = 0; i < vinfo.xres; i+=step)
        for(int j = 0; j < vinfo.yres; j+=step) 
            lcd_put_pixel(i,j,0xE066FF,vinfo,fbp);

    pthread_exit(NULL);
}


int main(char argc, char *argv[])
{
    int fd = open("/dev/fb0", O_RDWR);
    int framebuffer_size = 0;
    int step[THREAD_NUM];

    if(fd < 0)
    {
        perror("Error: cannot open framebuffer device");
        return 1;
    }

    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
    {
        perror("Error: cannot get variable screen information");
        return 1;
    }

    framebuffer_size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    fbp = (char*)mmap(NULL,framebuffer_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(fbp == MAP_FAILED)
    {
        perror("Error: failed to map framebuffer device to memory");
        return 1;
    }


    for(int i = 0; i < THREAD_NUM; i++)
    {
        step[i] = i + 1;
        pthread_create(&tid_buf[i],NULL,lcd_put_pixel_thr,&step[i]);    
    }

    for(int i = 0; i < THREAD_NUM; i++)
        pthread_join(tid_buf[i],NULL);    

    printf("All threads finished\n");

    munmap(fbp,framebuffer_size);
    close(fd);

    return 0;   

}