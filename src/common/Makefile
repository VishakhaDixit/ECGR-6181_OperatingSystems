SHELL := /bin/bash #use bash syntax

CC = g++
CFLAGS   = -O0 -Wall -std=c++11
INC_PATH = -I/usr/local/include/opencv4 -I../../include/
TARGET = common

OUTPUT_DIR = ../../build
OBJECT_DIR = $(OUTPUT_DIR)/$(TARGET)/obj

# Source files and dependencies
SRC = server.cpp image_convertor.cpp base64.cpp
OBJ += $(patsubst %.cpp, %.o, $(SRC))

LDFLAGS = $(LIBS) -L/usr/local/lib $(OPENCV_LIBS) -Wall -lstdc++fs

CFLAGS += $(INC_PATH) -c

#============================ RECEPIES ========================================

all: compile

compile: $(OBJ)

%.o: %.cpp
	@if [ ! -d $(OUTPUT_DIR) ];then mkdir $(OUTPUT_DIR);fi
	@if [ ! -d $(OUTPUT_DIR)/$(TARGET) ];then mkdir $(OUTPUT_DIR)/$(TARGET);fi
	@if [ ! -d $(OBJECT_DIR) ];then mkdir $(OBJECT_DIR);fi
	$(CC) $< $(CFLAGS) -o $(OBJECT_DIR)/$@


clean: 
	rm -rf $(OUTPUT_DIR)/$(TARGET)