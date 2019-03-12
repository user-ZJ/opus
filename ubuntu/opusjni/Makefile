TARGET := libopusjni.so

#指定交叉编译工具链
#CROSS_COMPILE = arm-hisiv100nptl-linux-
#AS      = $(CROSS_COMPILE)as  
#LD      = $(CROSS_COMPILE)ld  
#CC      = $(CROSS_COMPILE)gcc  
#CPP     = $(CC) -E  
#AR      = $(CROSS_COMPILE)ar  
#NM      = $(CROSS_COMPILE)nm
CC=gcc

#获取工程的根目录的全路径
SOURCE_ROOT = $(shell pwd)

#C/C++编译参数
CFLAGS  :=
CXXFLAGS:=

#链接库参数，-L 后面跟库文件的路径，  -l(小写)后面跟库的名字
#具体库名
LIBS    :=-lopus
#库所在目录
LDFLAGS := -L../../output/lib/




#-I是指定头文件的意思，所以这里是将所有的头文件的变量都包含在 INCLUDE_DIR 变量中
INCLUDE_DIR := -I$(SOURCE_ROOT)/ \
	-I/home/zack/jdk1.8.0_181/include/linux/ \
	-I/home/zack/jdk1.8.0_181/include/ \
	-I/home/zack/opus/opus-1.3/include/ \
	-I/home/zack/opus/opus-1.3/silk/ \
	-I/home/zack/opus/opus-1.3/celt/ \
	-I/home/zack/opus/opus-1.3/win32/


#源文件
SOURCE := cn_com_test_opus_OpusDecoder.c cn_com_test_opus_OpusEncoder.c 
#中间文件，patsubst把$(dir)中的变量符合后缀是.c的全部替换成.o
OBJS = $(patsubst %.c, %.o, $(SOURCE))
#OBJS  = $(foreach file, $(SOURCE), $(file) )

#link
$(TARGET):$(OBJS)
	$(CC) -shared -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

#$(OBJS):%.o:%.c
$(OBJS):$(SOURCE)
	$(CC) $(INCLUDE_DIR) $(CFLAGS) -c -fPIC $(SOURCE)

all:$(TARGET)

clean:
	@rm -f *.o *.so
.PHONY: clean
#gcc -I/home/zack/jdk1.8.0_181/include/linux/ -I/home/zack/jdk1.8.0_181/include/ -fPIC -shared -o libopusjni.so Main.c
