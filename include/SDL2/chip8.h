#pragma once
#include <stdint.h>
#include "memory.h"

struct chip8{
    struct chip8_memory memory;
    uint8_t V[0xF];
    uint16_t* I;
    uint16_t stack[0xF];
    uint16_t* sp;
    uint8_t sound;
    uint8_t delay;
};