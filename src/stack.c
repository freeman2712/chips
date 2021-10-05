#include "stack.h"
#include <stdlib.h>
struct Stack* initStack(){
    struct Stack* t = NULL;
    t = (struct Stack*)malloc(sizeof(struct Stack));
    t->SP = 0;
    return t;
}
void push_stack(struct Stack* stack, uint16_t val){
    stack->stack[(stack->SP)] = val;
    (stack->SP)++;
    return;
}
uint16_t pop_stack(struct Stack* stack){
    (stack->SP)--;
    uint16_t ret = stack->stack[stack->SP];

    return ret;

}
