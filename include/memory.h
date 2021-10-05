#include <stdint.h>
#ifndef MEMORY_H
#define MEMORY_H
#define MEMORY_SIZE 0x1000

struct Memory{
    uint8_t memory[MEMORY_SIZE];
};
void set_mem(struct Memory* memory, int index, int val);
uint8_t get_mem(struct Memory* memory, int index);
unsigned short get_opcode(struct Memory* memory, int pc);
#endif



