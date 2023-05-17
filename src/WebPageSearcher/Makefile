#当前路径
CUR_DIR = .#$(shell pwd)
#中间文件存放路径
OBJ_DIR = $(CUR_DIR)/obj

#通常项
CC = g++
# OUT = ../../bin/Cutter
OUT = ../main
FLAGS = -O3 -g -std=c++11 -lpthread
# SRCS = ./*.cc
SRCS = $(wildcard $(addsuffix /*.cc,$(CUR_DIR)))
#中间文件集合
# notdir ：除去文件名前的地址信息
# basename ：除去后缀名
# addprefix p1 p2 ： 把p1接到p2前 
# OBJ_DIR = ./
# OBJS = ./*.o
OBJS = $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(notdir $(SRCS)))))
OBJS += Configuration.o

INC_DIR  = .../../lib/cppjieba/include/cppjieba
INC_DIR  += ../../include/WebPageSearcher
INC_DIR  += ../../include/DictProducer
INC_DIR  += ../../include
# INC = -I .../../lib/cppjieba/include/cppjieba ...

INC  =  $(addprefix -I,$(INC_DIR))

ALL : $(OUT)
# OUT = ../../bin/Cutter OBJS = ./*.o
# g++ ./*.o -O3 -g -std=c++11 -lpthread -o ../../bin/Cutter
$(OUT) : $(notdir $(OBJS))
	$(CC) $(OBJ_DIR)/*.o $(FLAGS) -o $(OUT)
# “@”字符在命令行前，那么，这个命令将不被make显示出来
# $@--目标文件，$^--所有的依赖文件，$<--第一个依赖文件。
# 目标文件：依赖文件
# g++ ./*.cc -O3 -g -std=c++11 -lpthread -I .../../lib/cppjieba/include/cppjieba ... -c -o ./目标文件
%.o:%.cc
	$(CC) $< $(FLAGS) $(INC) -c -o $(OBJ_DIR)/$@
Configuration.o:../SearchEngineServer/Configuration.cc
	$(CC) $< $(FLAGS) $(INC) -c -o $(OBJ_DIR)/$@

.PHONY:clean rebuild
clean:
	rm -f $(OUT) $(OBJ_DIR)/*.o
rebuild:clean ALL