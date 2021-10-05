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
    uint16_t nnn = (opcode & 0x0FFF);
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t kk = (opcode & 0x00FF);
    uint8_t k = (opcode & 0x000F);
    unsigned short temp = 0;
    switch(opcode){
        case 0x00E0:
            op_0x00E0(c);
            break;
        case 0x00EE:
            op_0x00EE(c);
            break;
        default:
            switch(opcode & 0xF000){
                case 0x1000:
                    c->PC = nnn;
                    break;
                case 0x2000:
                    push_stack(c->stack, c->PC);
                    c->PC = nnn;
                    break;
                case 0x3000:
                    if(c->V[x] == kk){
                         c->PC = c->PC + 2;
                    }
                    break;

                case 0x4000:
                    if(c->V[x] != kk) c->PC = c->PC + 2;
                    break;
                case 0x5000:
                    if(c->V[x] == c->V[y])  c->PC = c->PC + 2;
                    break;
                case 0x6000:
                    c->V[x] = kk;
                    break;
                case 0x7000:
                    c->V[x] = c->V[x] + kk;
                    break;
                case 0x8000:
                    switch(k){
                        case 0x00:
                            c->V[x] = c->V[y];
                            break;
                        case 0x01:
                            c->V[x] = (c->V[x] | c->V[y]);
                            break;
                        case 0x02:
                            c->V[x] = c->V[x] & c->V[y];
                            break;
                        case 0x03:
                            c->V[x] = c->V[x] ^ c->V[y];
                            break;
                        case 0x04:
                            if((int)c->V[x] + (int)c->V[y] < 256){
                                c->V[0xF] = 0;
                            }
                            else{
                                c->V[0xF] = 1;
                            }
                            c->V[x] += c->V[y];
                            break;
                        case 0x05:
                            if((int)c->V[x]>(int)c->V[y]) c->V[0xF] = 1;
                            else                c->V[0xF] = 0; 
                            c->V[x] = c->V[x] - c->V[y];
                            break;
                        case 0x06:
                            c->V[0x0F] = c->V[x] & 7;
                            c->V[x] = c->V[x]>>1;
                            break;
                        case 0x07:
                        {
                            if(((int)c->V[x] - (int)c->V[y]) > 0){
                                c->V[0xF] = 1;
                            }
                            else{
                                c->V[0xf] &= 0;
                            }
                            c->V[x] = c->V[y] - c->V[x];
                        }
                            break;
                        case 0x0E:
                        {
                            c->V[0xF] = c->V[x] >> 7;
                            c->V[x] = c->V[x]<<2;
                        }
                            break;
                        break;


                    }
                case 0x9000:
                    if(c->V[x] != c->V[y])  c->PC = c->PC + 2;
                    break;

                case 0xA000:
                    c->I = nnn;
                    break;
                case 0xB000:
                    c->PC = c->V[0x00] + nnn;
                    break;
                case 0xC000:
                    srand(clock());
                    c->V[x] = kk & (rand()%255);
                    break;
                case 0xD000:
                    c->V[0x0F] = screen_draw_sprite(c->sc, c->V[x], c->V[y], (unsigned const char*)&(c->memory).memory[c->I], k);
                    break;
                case 0xE000:
                    switch(kk){
                        case 0x00E:
                            if(is_key_down(c->kb, c->V[x]))     c->PC +=2;
                            break;
                        case 0x001:
                            if(!is_key_down(c->kb, c->V[x]))    c->PC += 2;
                            break;
                    }
                    
                    break;
                case 0xF000:
                    switch(kk){
                        case 0x07:
                            c->V[x] = c->delay;
                            break;
                        case 0x0A:
                        {
                            c->V[x] = keyBug(c, x);
                        }
                            break;
                        case 0x15:
                            c->delay = c->V[x];
                            break;
                        case 0x18:
                            c->sound = c->V[x];
                            break;
                        case 0x1E:
                            c->I += c->V[x];
                            break;
                        case 0x29:
                            c->I = c->V[x] * 5;
                            break;
                        case 0x33:
                        {
                            set_mem(&(c->memory), c->I, (c->V[x])/100);
                            set_mem(&(c->memory), (c->I) + 1, (c->V[x]/10)%10);
                            set_mem(&(c->memory), (c->I) + 2, c->V[x]%10);
                        }
                            break;
                        case 0x55:
                            for(int i = 0; i<=x; i++)   set_mem(&(c->memory), c->I + i, c->V[i]);
                            break;
                        case 0x65:
                            for(int i = 0; i<=x; i++){
                                c->V[i] = get_mem(&(c->memory), c->I + i);
                            }

                            break;

                    }
                    break;





            }
        
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