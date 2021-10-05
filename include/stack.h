#include <stdint.h>
#ifndef STACK_H
#define STACK_H
#define STACK_SIZE 0x10
struct Stack{
    uint16_t stack[STACK_SIZE + 1];
    uint16_t SP;
};
struct Stack* initStack();
void push_stack(struct Stack* stack, uint16_t val);
uint16_t pop_stack(struct Stack* stack);

#endif



