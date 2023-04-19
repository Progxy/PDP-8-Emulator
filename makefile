# OBJS specifies which files to compile as part of the project
OBJS = main.c

# CC specifies which compiler we're using
CC = gcc

# Headers files
HEADERS = compiler.c isa.c utils.c emulator.c analyzer.c

# COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -std=c11 -Wall

# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = pdp8

# LIB_FLAGS specifies the additional library to link
# LIB_FLAGS = 

all : $(OBJS)
	$(CC) $(OBJS) $(HEADERS) $(COMPILER_FLAGS) -o $(OBJ_NAME)