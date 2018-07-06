DIR = $(shell pwd)
PROGRAM = $(DIR)/canny
SOURCE = $(DIR)/

INCLUDE = -I$(DIR)

CC ?= g++
CFLAGS = -Wall -g -O0
LDFLAGS = -lstdc++ -lopenvx `pkg-config --libs --cflags opencv`

all:$(SOURCE)canny.cpp 
	$(CC) $(SOURCE)canny.cpp $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(PROGRAM)
		
.PHONY:clean	
clean:
	rm $(PROGRAM)
