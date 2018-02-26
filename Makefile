
# https://docs.opencv.org/2.4/doc/tutorials/introduction/linux_gcc_cmake/linux_gcc_cmake.html
# https://youtu.be/dqflr7_TqQ8
# https://stackoverflow.com/a/30602701

CC = g++
# CFLAGS = $(shell pkg-config --cflags opencv) #-Wall
# LIBS = $(shell pkg-config --libs opencv)
SRC_DIR := src
BIN_DIR := bin
INC_DIR := include

CFLAGS := -std=c++11 # `pkg-config --cflags opencv` -I$(INC_DIR)#-Wall
LIBS := #`pkg-config --libs opencv`
EXEC := screenbrightness

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# g++ main.cpp -o test `pkg-config --cflags --libs opencv`
all: $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(LIBS)

help:
	@echo "src: $(SRC)"
	@echo "obj: $(OBJ)"
	@echo "bin: $(BIN_DIR)/$(EXEC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LIBS: $(LIBS)"

clean:
	@rm -rf $(BIN_DIR)/*
	@echo "Cleaned $(BIN_DIR)/*"


