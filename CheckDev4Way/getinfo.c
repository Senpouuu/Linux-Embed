/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2024-12-18 21:37:30
 * @description    :  获取 ev 设备支持的 event 类型 
 *------------------------------------------------------------------------**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

int main()
{
    struct input_id ei_id;
    int fd = open("/dev/input/event1", O_RDONLY);
    int err;
    unsigned long len;
    char *ev_types[] = {"EV_SYN",
                      "EV_KEY",
                      "EV_REL",
                      "EV_ABS",
                      "EV_MSC",
                      "EV_SW",
                      "EV_NONE",
                      "EV_NONE",
                      "EV_NONE",
                      "EV_NONE",
                      "EV_NONE",
                      "EV_LED",
                      "EV_SND",
                      "EV_NONE",
                      "EV_REP",
                      "EV_FF",
                      "EV_PWR",
                      "EV_FF_STATUS"};

    if (fd < 0)
    {
        perror("open");
        exit(1);
    }

    err = ioctl(fd, EVIOCGID, &ei_id);
    if (err < 0)
    {
        perror("ioctl");
        exit(1);
    }
    printf("Bus: %hu\n", ei_id.bustype);
    printf("Vendor: %hu\n", ei_id.vendor);
    printf("Product: %hu\n", ei_id.product);
    printf("Version: %hu\n", ei_id.version);

    err = ioctl(fd, EVIOCGBIT(EV_SYN, sizeof(len)), &len);
    if (err < 0)
    {
        perror("ioctl");
        exit(1);
    }

 
    for (int j = 0; j < 32; j++)
    {
        if (len & 1 << j)
        {
            printf("bit %d is not 0\n", j);
            printf("Support %s\n", ev_types[j]);
        }
    }

    close(fd);

    return 0;
}