/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2024-12-26 15:04:11
 * @description    :  用TSlib 在LCD上绘图
 *------------------------------------------------------------------------**/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <pthread.h>
#include <signal.h>
#include <tslib.h>

#define THREAD_NUM 5 


static pthread_t tid;
static pthread_t fscreen_tid_buf[THREAD_NUM];
static int thr_inx[THREAD_NUM];

static struct fb_var_screeninfo var;
static char *fb_base;

static struct tsdev *ts;
static struct ts_sample* sample;
static struct ts_sample* sample_old;

void lcd_put_pixel(int x, int y, unsigned int color) 
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

void draw_line(int x1, int y1, int x2, int y2, unsigned int color)
{
    // printf("drawing line (%d,%d) -> (%d,%d) with color %x\n",x1,y1,x2,y2,color);
    
    if((x1 == 0 && y1 == 0) || (x2 == 0 && y2 == 0))
        return;
    
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        lcd_put_pixel(x1, y1, color); // 绘制当前点

        // 如果到达终点，结束循环
        if (x1 == x2 && y1 == y2) break;

        // 计算误差
        int err2 = err * 2;

        // 更新 x 和 y 坐标
        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}


int lcd_init() 
{
    int fd = open("/dev/fb0", O_RDWR);
    int fb_len; 

    if (fd < 0) 
    {
        perror("open fb0 failed");
        return -1;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &var) < 0) 
    {
        perror("get vscreeninfo failed");
        close(fd);
        return -1;
    }

    fb_len = var.xres * var.yres * var.bits_per_pixel / 8;
    fb_base = mmap(0, fb_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fb_base == MAP_FAILED) 
    {
        perror("mmap failed");
        close(fd);
        return -1;
    }

    for(int i = 0; i < THREAD_NUM; i++)
        thr_inx[i] = i+1;

    return 0;

}


void* thr_lcd_refresh(void *arg) 
{
    int step = *(int*)arg;    
    printf("Thread %d start\n",step);
    for(int i = 0; i < var.xres; i+=step)
        for(int j = 0; j < var.yres; j+=step) 
            lcd_put_pixel(i,j,0);

    pthread_exit(NULL);
}


void lcd_refresh() 
{
    for(int i = 0; i < THREAD_NUM; i++)
        pthread_create(&fscreen_tid_buf[i], NULL, thr_lcd_refresh, &thr_inx[i]);

}


void sig_func(int sig) 
{
    munmap(fb_base, var.xres * var.yres * var.bits_per_pixel / 8);  
    printf("exit\n");
    exit(0);
}




int main()
{
    int ret;
    lcd_init();
    lcd_refresh();
    signal(SIGINT, sig_func);
    for(int i = 0; i < THREAD_NUM; i++) 
        pthread_join(fscreen_tid_buf[i], NULL);

    ret = fork();
    if(ret < 0)
    {
        perror("fork failed");
        exit(1);
    }
    
    if(ret == 0) //child process
    {
        printf("Input command: <clr> to clear screen...\n");
        char cmd[10];
        while(1)
        {
            scanf("%s", cmd);
            if(strcmp(cmd, "clr") == 0)
                lcd_refresh();
        }

    }

    if(ret > 0) //parent process
    {
        ts = ts_setup(NULL,0);
        if(ts == NULL)
        {
            perror("ts_setup failed");
            exit(1);
        }
        sample = calloc(1,sizeof(struct ts_sample));
        sample_old = calloc(1,sizeof(struct ts_sample));
        if(sample == NULL)
        {
            perror("malloc failed");
            exit(1);
        }

        while(1)
        {
            // printf("*****************************\n");
            if(ts_read(ts, sample, 1) == 1)
            {
                // printf("touchscreen (%d,%d) with pressure %d\n",sample->x,sample->y,sample->pressure);
                // printf("sample (%d,%d) with pressure %d\n",sample->x,sample->y,sample->pressure);
                // printf("sample_old (%d,%d) with pressure %d\n",sample_old->x,sample_old->y,sample_old->pressure);
                if(sample->pressure == 0)
                {
                    memset(sample,0,sizeof(struct ts_sample));
                    memset(sample_old,0,sizeof(struct ts_sample));
                    continue;
                }
                draw_line(sample_old->x,sample_old->y,sample->x,sample->y,0xffffff);
                sample_old->x = sample->x;
                sample_old->y = sample->y;
            }
        }
    }
    

    
}