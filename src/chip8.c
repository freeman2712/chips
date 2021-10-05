#include "chip8.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
const char keymap[] = {
SDLK_1,
SDLK_2,
SDLK_3,
SDLK_q,
SDLK_w,
SDLK_e,
SDLK_a,
SDLK_s,
SDLK_d,
SDLK_x,
SDLK_z,
SDLK_c,
SDLK_4,
SDLK_r,
SDLK_f,
SDLK_v
};
unsigned const char chip8_charmap[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90, 
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80,
};

struct Chip8* initChip8(){
    struct Chip8* c = NULL;
    c = (struct Chip8*)malloc(sizeof(struct Chip8));
    for(int i = 0; i<MEMORY_SIZE; i++)    c->memory.memory[i] = 0x0;
    memcpy(&(c->memory.memory), chip8_charmap, sizeof(chip8_charmap));
    c->sc = initScreen();
    c->stack = initStack();
    c->kb = initKey();
    for(int i = 0; i<16; i++)    c->V[i] = 0;
    c->delay = 0;
    c->I = 0;
    c->PC = 0x200;
    c->sound = 0;
    return c;
}
void load(struct Chip8* c, unsigned char* buffer, int size){
    memcpy(&(c->memory.memory[0x200]), buffer, size);
    c->PC = 0x200;
    return;
}
void exec(struct Chip8* c, unsigned short opcode){
        switch(opcode & 0xF000){    
            case 0x0000:
                switch(opcode & 0x000F){
                    case 0x0000:
                        op_0x00E0(c);
                        c->PC += 2;
                    break;
         
                    case 0x000E:    
                           op_0x00EE(c);
                           c->PC = c->PC + 2;
                    break;  
                       
                }
            break;
           
            case 0x1000:
                c->PC = opcode & 0x0FFF;
            break;  
               
            case 0x2000:
                c->stack->stack[((c->stack->SP)++)&0xF] = c->PC;
                c->PC = opcode & 0x0FFF;
            break;
       
            case 0x3000: 
                if(c->V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                    c->PC += 4;
                else
                    c->PC += 2;
            break;
       
            case 0x4000:
                if(c->V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                    c->PC += 4;
                else
                    c->PC += 2;
            break;
       
            case 0x5000: 
                if(c->V[(opcode & 0x0F00) >> 8] == c->V[(opcode & 0x00F0) >> 4])
                    c->PC += 4;
                else
                    c->PC += 2;
            break;
       
            case 0x6000: 
                c->V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
                c->PC += 2;
            break;
       
            case 0x7000: 
                c->V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
                c->PC += 2;
            break;
               
            case 0x8000:
                switch(opcode & 0x000F){
                    case 0x0000: // 8XY0: Sets VX to the value of VY
                        c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x00F0) >> 4];
                        c->PC += 2;
                    break;
       
                    case 0x0001: // 8XY1: Sets VX to VX or VY
                        c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x0F00) >> 8] | c->V[(opcode & 0x00F0) >> 4];
                        c->PC += 2;
                    break;
       
                    case 0x0002: // 8XY2: Sets VX to VX and VY
                        c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x0F00) >> 8] & c->V[(opcode & 0x00F0) >> 4];
                        c->PC += 2;
                    break;
       
                    case 0x0003: // 8XY3: Sets VX to VX xor VY
                        c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x0F00) >> 8] ^ c->V[(opcode & 0x00F0) >> 4];
                        c->PC += 2;
                    break;
       
                    case 0x0004: // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
                        if(((int)c->V[(opcode & 0x0F00) >> 8 ] + (int)c->V[(opcode & 0x00F0) >> 4]) < 256)
                            c->V[0xF] &= 0;
                        else
                            c->V[0xF] = 1;

                        c->V[(opcode & 0x0F00) >> 8] += c->V[(opcode & 0x00F0) >> 4];
                        c->PC += 2;
                    break;
       
                    case 0x0005:
                        if(((int)c->V[(opcode & 0x0F00) >> 8 ] - (int)c->V[(opcode & 0x00F0) >> 4]) >= 0)
                            c->V[0xF] = 1;
                        else
                            c->V[0xF] &= 0;

                        c->V[(opcode & 0x0F00) >> 8] -= c->V[(opcode & 0x00F0) >> 4];
                        c->PC += 2;
                    break;
       
                    case 0x0006:
                        c->V[0xF] = c->V[(opcode & 0x0F00) >> 8] & 7;
                        c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x0F00) >> 8] >> 1;
                        c->PC += 2;
                    break;
       
                    case 0x0007: 
                        if(((int)c->V[(opcode & 0x0F00) >> 8] - (int)c->V[(opcode & 0x00F0) >> 4]) > 0)
                            c->V[0xF] = 1;
                        else
                            c->V[0xF] &= 0;

                        c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x00F0) >> 4] - c->V[(opcode & 0x0F00) >> 8];
                        c->PC += 2;
                    break;
       
                    case 0x000E: 
                        c->V[0xF] = c->V[(opcode & 0x0F00) >> 8] >> 7;
                        c->V[(opcode & 0x0F00) >> 8] = c->V[(opcode & 0x0F00) >> 8] << 1;
                        c->PC += 2;
                    break;
                         
                }
            break;
       
            case 0x9000: 
                if(c->V[(opcode & 0x0F00) >> 8] != c->V[(opcode & 0x00F0) >> 4])
                    c->PC += 4;
                else
                    c->PC += 2;
            break;
       
            case 0xA000:
                c->I = opcode & 0x0FFF;
                c->PC += 2;
            break;
       
            case 0xB000: 
                c->PC = (opcode & 0x0FFF) + c->V[0];
            break;
       
            case 0xC000:
                c->V[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF);
                c->PC += 2;
            break;
       
            case 0xD000: 
                c->V[0x0F] = screen_draw_sprite(c->sc, c->V[(opcode & 0x0F00) >> 8], c->V[(opcode & 0x00F0) >> 4], (unsigned const char*)&(c->memory).memory[c->I], (opcode & 0x000F));
                c->PC += 2;
            break;
       
            case 0xE000:
                switch(opcode & 0x000F){
                    case 0x000E:
           		        if(is_key_down(c->kb, c->V[(opcode & 0x0F00) >> 8]))     c->PC +=4;
  			            else					                                 c->PC = c->PC + 2;
                   	 break;
                             
                    case 0x0001:
                        if(!is_key_down(c->kb, c->V[(opcode & 0x0F00) >> 8]))
                            c->PC += 4;
                        else
                            c->PC += 2;
                    break;
                         
                }
            break;
       
            case 0xF000:
                switch(opcode & 0x00FF){
                    case 0x0007:
                        c->V[(opcode & 0x0F00) >> 8] = c->delay;
                        c->PC += 2;
                    break;
       
                    case 0x000A:
    			c->V[(opcode & 0x0F00) >> 8] = keyBug(c, 1);
                    break;
       
                    case 0x0015:
                        c->delay = c->V[(opcode & 0x0F00) >> 8];
                        c->PC += 2;
                    break;
       
                    case 0x0018: 
                        c->sound = c->V[(opcode & 0x0F00) >> 8];
                        c->PC += 2;
                    break;
       
                    case 0x001E: 
                        c->I += c->V[(opcode & 0x0F00) >> 8];
                        c->PC += 2;
                    break;

                    case 0x0029:
                        c->I = c->V[(opcode & 0x0F00) >> 8] * 5;
                        c->PC += 2;
                    break;
       
                    case 0x0033:
                        set_mem(&(c->memory), c->I, (c->V[(opcode & 0x0F00) >> 8]) / 100);
                        set_mem(&(c->memory), (c->I) + 1, (c->V[(opcode & 0x0F00) >> 8] / 10) % 10);
                        set_mem(&(c->memory), (c->I) + 2, c->V[(opcode & 0x0F00) >> 8] % 10);
                        c->PC += 2;
                        break;
       
                    case 0x0055: 
                        for(int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
                            c->memory.memory[c->I+i] = c->V[i];
                        c->PC += 2;
                    break;
       
                    case 0x0065: 
                        for(int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
                            c->V[i] = c->memory.memory[c->I + i];
                        c->PC += 2;
                    break;
                         

                }
            break;

                 
        }
}
void op_0x00E0(struct Chip8* c){
    memset((c->sc)->pixels, 0, sizeof((c->sc)->pixels));
    return;
}
void op_0x00EE(struct Chip8* c){
    c->PC = pop_stack(c->stack);
    return;

}
int keyBug(struct Chip8* c, int x){
    SDL_Event event;
    int ans = 0;
    int flag = 0;
    while (SDL_WaitEvent(&event))
    {
        if (event.type != SDL_KEYDOWN)
            continue;
        char a = event.key.keysym.sym;
            for(int i = 0; i<16; i++){
                if(keymap[i] == a)
                {
                    ans = i;
                    flag = 1;
                    break;
                }
            }
            if(flag)    break;
    }
    return ans;
}