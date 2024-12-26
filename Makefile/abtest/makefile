CFLAGS = -Wall -g

SRCS := $(wildcard *.c)
DEPENDS:=$(SRCS:.c=.d)
OBJS:=$(SRCS:.c=.o)

# 此句将会重复执行,每次解析都会执行一次
DEPENDS := $(wildcard $(DEPENDS))

test: $(OBJS)
	gcc -o $@ $^

ifneq ($(DEPENDS),)
-include $(DEPENDS)
endif

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d

print:
	@echo $(DEPENDS)
	@echo $(OBJS)
	@echo $(SRCS)

clean:
	rm -f *.o .*.d test 

.PHONY:clean

