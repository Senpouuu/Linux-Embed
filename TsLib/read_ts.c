/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2024-12-21 18:05:17
 * @description    :  使用tslib库测量两指距离
 *------------------------------------------------------------------------**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tslib.h>
#include <math.h>


#define MAX_SLOTS 5
#define NUM_SLOTS 2


int calculateDistance(int x1, int y1, int x2, int y2) {
    // 计算差值
    int dx = x2 - x1;
    int dy = y2 - y1;

    // 计算距离并取整
    return (int)sqrt(dx * dx + dy * dy);
}

int main()
{
    struct tsdev *ts;
    struct ts_sample_mt** sample_mt;
    int ret;
    ts = ts_setup(NULL,0);
    if(ts == NULL)
    {
        perror("ts_setup");
        exit(1);
    }

    sample_mt = malloc(NUM_SLOTS * sizeof(struct ts_sample_mt*));
    if(sample_mt == NULL)
    {
        perror("malloc");
        exit(1);
    }

    for(int i = 0; i < NUM_SLOTS; i++)
    {
        sample_mt[i] = calloc(MAX_SLOTS, sizeof(struct ts_sample_mt));
        if(sample_mt[i] == NULL)
        {
            perror("malloc");
            exit(1);
        }
    }

    int i, j;

    struct touch_dis_st
    {
        int x;
        int y;
        int slot;
    };

    struct touch_dis_st touch_dis[NUM_SLOTS];

    for(i = 0; i < NUM_SLOTS; i++)
        touch_dis[i].slot = -1; 

    int distance = 0;

    while(1)
    {  
        ret = ts_read_mt(ts, sample_mt, MAX_SLOTS,NUM_SLOTS);
        for(i = 0; i < ret; i++)
            for(j = 0; j < MAX_SLOTS; j++)
            {
                if(!sample_mt[i][j].valid)
                    continue;

                if (touch_dis[i].slot == -1)
                    touch_dis[i].slot = j;

                touch_dis[i].x = sample_mt[i][touch_dis[i].slot].x;
                touch_dis[i].y = sample_mt[i][touch_dis[i].slot].y;
            }
        distance = calculateDistance(touch_dis[0].x, touch_dis[0].y, touch_dis[1].x, touch_dis[1].y);
        printf("Distance between two fingers: %d\n", distance);
    }

    exit(0);
}

/*---------------------------- END OF SECTION 2024-12-23 22:40:27 ----------------------------*/
