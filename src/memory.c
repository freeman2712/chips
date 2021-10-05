#include "memory.h"




void set_mem(struct Memory* memory, int index, int val){
    if(index>0xFFF) return;
    memory->memory[index] = val;
    return;
}
uint8_t get_mem(struct Memory* memory, int index){
    if(index>0xFFF) return 0;
    return memory->memory[index];

}
unsigned short get_opcode(struct Memory* memory, int pc)
{
    unsigned short opcode = 0;
    uint8_t byte_1 = get_mem(memory, pc);
    uint8_t byte_2 = get_mem(memory, pc + 1);
    opcode = (byte_1 << 8) | byte_2;
    return opcode;
}