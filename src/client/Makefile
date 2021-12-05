SHELL := /bin/bash #use bash syntax

CC = g++
CFLAGS   = -O0 -Wall -std=c++11
INC_PATH = -I/usr/local/include/opencv4 -I../../include/
EXT = out
TARGET = client

OPENCV_LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d \
			  -lopencv_calib3d -lopencv_objdetect -lopencv_flann -lopencv_imgcodecs -lopencv_videoio

LIBS = -lpthread

OUTPUT_DIR = ../../build
OBJECT_DIR = $(OUTPUT_DIR)/obj

EXE = $(OUTPUT_DIR)/$(PRGM).$(EXT)

# Source files and dependencies
SRC = client.cpp main.cpp
OBJ += $(patsubst %.cpp, %.o, $(SRC))

LDFLAGS = $(LIBS) $(OPENCV_LIBS) -Wall -lstdc++fs

CFLAGS += $(INC_PATH) -c

#============================ RECEPIES ========================================

all: bin

compile: $(OBJ)
execfile: $(TARGET).$(EXT)
bin: $(TARGET).$(EXT)

%.o: %.cpp
	@if [ ! -d $(OBJECT_DIR) ];then mkdir $(OBJECT_DIR);fi
	$(CC) $< $(CFLAGS) -o $(OBJECT_DIR)/$@


$(TARGET).$(EXT): $(OBJ)
	$(CC) -L/usr/local/lib $(LDFLAGS) $(OBJECT_DIR)/client.o $(OBJECT_DIR)/main.o -o $(OUTPUT_DIR)/$(TARGET).$(EXT)


clean: 
	rm -rf $(OUTPUT_DIR)/*