CC = gcc
FLAGS= -Wall -g -m32
OBJECT= ./build/memory.o ./build/stack.o ./build/keyboard.o ./build/chip8.o ./build/screen.o
all: $(OBJECT)
	$(CC) $(FLAGS) -I ./include ./src/main.c ${OBJECT} -L ./lib -lSDL2main -lSDL2 -o ./bin/main

./build/memory.o: ./src/memory.c
	$(CC) $(FLAGS) -I ./include ./src/memory.c -c -o ./build/memory.o

./build/stack.o: ./src/stack.c
	$(CC) $(FLAGS) -I ./include ./src/stack.c -c -o ./build/stack.o

./build/keyboard.o: ./src/keyboard.c
	$(CC) $(FLAGS) -I ./include ./src/keyboard.c -c -o ./build/keyboard.o

./build/chip8.o: ./src/chip8.c
	$(CC) $(FLAGS) -I ./include ./src/chip8.c -c -o ./build/chip8.o

./build/screen.o: ./src/screen.c
	$(CC) $(FLAGS) -I ./include ./src/screen.c -c -o ./build/screen.o

clean:
	del .\build\*