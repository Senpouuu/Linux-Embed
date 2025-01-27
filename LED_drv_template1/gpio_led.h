#ifndef GPIO_LED_H_
#define GPIO_LED_H_

void gpio_led_mmap(void);

void gpio_led_munmap(void);

void gpio_led_init(void);

void gpio_led_set(int value);

char gpio_led_get(void);

void gpio_led_exit(void);


#endif