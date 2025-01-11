# 子目录列表
SUBDIRS := Framebuffer CheckDev4Way DrawLCD I2C SPI TsLib UART Web

# 全局编译选项
CFLAGS := -I/home/senpouuu/100ask_imx6ull_mini-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include \
          -D_POSIX_SOURCE \
          -D_XOPEN_SOURCE=600 \
          -D_DEFAULT_SOURCE \
		  -march=armv7-a 
         
LDFLAGS := 

# 默认目标
.PHONY: all clean bear

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

bear:
	@rm -f compile_commands.json
	@for dir in $(SUBDIRS); do \
		(cd $$dir && bear -- $(MAKE) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"); \
	done
	@find $(SUBDIRS) -name compile_commands.json -exec jq -s 'add' {} + > compile_commands.json
	@find $(SUBDIRS) -name compile_commands.json -exec rm -f {} +
