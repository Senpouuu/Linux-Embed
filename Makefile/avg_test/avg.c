#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define TYPE int
#define AVG_TYPE double

struct avg_buff_st
{
    TYPE buffer[BUFFER_SIZE];
    int len;
    int pos;
    AVG_TYPE avg;
};

void* avg_buff_init()
{
    struct avg_buff_st* avg_buff = (struct avg_buff_st*)malloc(sizeof(struct avg_buff_st));
    avg_buff->len = 0;
    avg_buff->pos = 0;
    avg_buff->avg = 0;
    return avg_buff;
}

int avg_buff_isfull(void* avg_buff)
{
    struct avg_buff_st* p_avg_buff = (struct avg_buff_st*)avg_buff;
    return p_avg_buff->len == BUFFER_SIZE;
}

int avg_buff_avg(void* avg_buff)
{
    if(!avg_buff_isfull(avg_buff))
        return -1;

    AVG_TYPE sum = 0;
    struct avg_buff_st* p_avg_buff = (struct avg_buff_st*)avg_buff;
    for(int i = 0; i < BUFFER_SIZE; i++)
        sum += p_avg_buff->buffer[i];

    p_avg_buff->avg = sum / BUFFER_SIZE;
    return 1;

}

void avg_buff_add(void* avg_buff, TYPE value)
{
    printf("ADD ************** value = %d\n", value);
    struct avg_buff_st* p_avg_buff = (struct avg_buff_st*)avg_buff;
    
    if(p_avg_buff->pos == BUFFER_SIZE)
        p_avg_buff->pos = 0;

    p_avg_buff->buffer[p_avg_buff->pos] = value;
    p_avg_buff->pos += 1;

    if(p_avg_buff->len < BUFFER_SIZE)
        p_avg_buff->len += 1;
    
    if(avg_buff_isfull(avg_buff))
        avg_buff_avg(avg_buff);
    
    return;
}


void avg_buff_print(void* avg_buff)
{
    struct avg_buff_st* p_avg_buff = (struct avg_buff_st*)avg_buff;
    printf("avg_buff->len = %d\n", p_avg_buff->len);
    printf("avg_buff->pos = %d\n", p_avg_buff->pos);
    printf("avg_buff->avg = %f\n", p_avg_buff->avg);
    for(int i = 0; i < BUFFER_SIZE; i++)
        printf("avg_buff->buffer[%d] = %d\n", i, p_avg_buff->buffer[i]);
}

int main() 
{
    void* avg_buff = avg_buff_init();
    avg_buff_add(avg_buff, 1);
    avg_buff_add(avg_buff, 2);
    avg_buff_add(avg_buff, 3);
    
    avg_buff_print(avg_buff);

    avg_buff_add(avg_buff, 4);
    avg_buff_print(avg_buff);

    avg_buff_add(avg_buff, 5);
    avg_buff_print(avg_buff);

    avg_buff_add(avg_buff, 6);
    avg_buff_print(avg_buff);

    avg_buff_add(avg_buff, 6);
    avg_buff_print(avg_buff);

    avg_buff_add(avg_buff, 8);
    avg_buff_print(avg_buff);
}