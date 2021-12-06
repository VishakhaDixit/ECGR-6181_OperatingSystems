SHELL := /bin/bash #use bash syntax

CC = g++
CFLAGS   = -O0 -Wall -std=c++11
INC_PATH = -I/usr/local/include/opencv4 -I../../include/
EXT = out
TARGET = simpleServer

OPENCV_LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d \
			  -lopencv_calib3d -lopencv_objdetect -lopencv_flann -lopencv_imgcodecs -lopencv_videoio

LIBS = -lpthread

OUTPUT_DIR = ../../build
OBJECT_DIR = $(OUTPUT_DIR)/$(TARGET)/obj

EXE = $(OUTPUT_DIR)/$(PRGM).$(EXT)

# Source files and dependencies
SRC = server.cpp main.cpp image_convertor.cpp base64.cpp simple_server.cpp
OBJ += $(patsubst %.cpp, %.o, $(SRC))

LDFLAGS = $(LIBS) -L/usr/local/lib $(OPENCV_LIBS) -Wall -lstdc++fs

CFLAGS += $(INC_PATH) -c

#============================ RECEPIES ========================================

all: bin

compile: $(OBJ)
execfile: $(TARGET).$(EXT)
bin: $(TARGET).$(EXT)

%.o: %.cpp
	@if [ ! -d $(OUTPUT_DIR) ];then mkdir $(OUTPUT_DIR);fi
	@if [ ! -d $(OUTPUT_DIR)/$(TARGET) ];then mkdir $(OUTPUT_DIR)/$(TARGET);fi
	@if [ ! -d $(OBJECT_DIR) ];then mkdir $(OBJECT_DIR);fi
	$(CC) $< $(CFLAGS) -o $(OBJECT_DIR)/$@


$(TARGET).$(EXT): $(OBJ)
	$(CC) $(OBJECT_DIR)/*.o -o $(OUTPUT_DIR)/$(TARGET).$(EXT) $(LDFLAGS)


clean: 
	rm -rf $(OUTPUT_DIR)/$(TARGET)
	rm -f $(OUTPUT_DIR)/$(TARGET).$(EXT)