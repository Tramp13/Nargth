OBJS = main.c camera.c sprites.c map.c quad.c entity.c timer.c

CC = gcc

#COMPILER_FLAGS = -w

LINKER_FLAGS = -lm -lSDL2 -lSDL2_image

OBJ_NAME = main

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
