#目标（要生成的文件名）
TARGET     := test

#编译器的选择（在Linux中其实可以忽略，因为cc指向的本来就是gcc）
#CC     := arm-xilinx-linux-gnueabi-g++
#CC     := aarch64-openwrt-linux-gcc
CC     := gcc

#链接参数
LDFLAG     := -lpthread

#编译包含的头文件所在目录
INCLUDES   := -I.

#所有用到的源文件，注意：非当前目录的要+上详细地址
SRCS    = $(wildcard *.c)

#把源文件SRCS字符串的后缀.c改为.o
OBJS    = $(SRCS:.c=.o)

#匹配所有的伪目标依赖，即执行目标myhello.o & ./common/abc.c & ./common/test/test.c   
#all为伪目标all
#当所有依赖目标都存在后，链接，即链接myhello.o & ./common/abc.c & ./commontest/test.c  
.PHONY:all
all:$(OBJS)
	$(CC) -o $(TARGET) $^ $(LDFLAG)

#重定义隐藏规则，匹配上述目标：myhello.o & ./common/abc.c & ./common/test/test.c  
#生成.o文件，注意，由于SRCS有个别包含详细地址的，生成的.o文件也是详细地址  
%.o:%.c
	$(CC) -c $(INCLUDES) $< -o $@

#清空除源文件外的所有生成文件
clean:
	rm -rf $(basename $(TARGET)) $(SRCS:.c=.o)
