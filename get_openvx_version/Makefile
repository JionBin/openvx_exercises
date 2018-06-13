DIR = $(shell pwd)
PROGRAM = $(DIR)/build/openvx_version
SOURCE = $(DIR)/src/

INCLUDE = -I$(DIR)

CC = g++
CFLAGS = -w -g -O0
LDFLAGS = -lstdc++ -lopenvx 

all:$(SOURCE)openvx_version.cpp 
	$(CC) $(SOURCE)openvx_version.cpp $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(PROGRAM)
		
.PHONY:clean	
clean:
	rm $(PROGRAM)
