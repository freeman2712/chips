
#include <stdint.h>
#ifndef CHIP8_H
#define CHIP8_H
#include "memory.h"
#include "stack.h"
#include "keyboard.h"
#include "screen.h"
#include "SDL2/SDL.h"
struct Chip8{
    struct Memory memory;
    struct Stack* stack;
    struct Keyboard* kb;
    struct Screen* sc;
    uint8_t V[0xF];
    uint16_t I;
    uint16_t PC;
    uint8_t sound;
    uint8_t delay;
};
struct Chip8* initChip8(); 
void load(struct Chip8* c, unsigned char* buffer, int size);
void exec(struct Chip8* c, unsigned short opcode);
void op_0x00E0(struct Chip8*c);
int keyBug(struct Chip8* c, int x);
void op_0x00EE(struct Chip8*c);
#endif
