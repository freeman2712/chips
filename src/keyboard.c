#include "keyboard.h"
#include <stdlib.h>
#include "SDL2/SDL.h"

struct Keyboard* initKey(){
    struct Keyboard* k = NULL;
    k = (struct Keyboard*)malloc(sizeof(struct Keyboard));
    for(int i = 0; i<KEY_SIZE; i++)  k->key_press[i] = false;
    k->map[0] = SDLK_1;
    k->map[1] = SDLK_2;
    k->map[2] = SDLK_3;
    k->map[3] = SDLK_q;
    k->map[4] = SDLK_w;
    k->map[5] = SDLK_e;
    k->map[6] = SDLK_a;
    k->map[7] = SDLK_s;
    k->map[8] = SDLK_d;
    k->map[9] = SDLK_x;
    k->map[0xa] = SDLK_z;
    k->map[0xb] = SDLK_c;
    k->map[0xc] = SDLK_4;
    k->map[0xd] = SDLK_r;
    k->map[0xe] = SDLK_f;
    k->map[0xf] = SDLK_v; 
    return k;
}
int key_map(struct Keyboard* keyboard, char key){
    for(int i = 0; i<KEY_SIZE; i++)
        if(keyboard->map[i] == key) return i;
    return -1;
}
void key_down(struct Keyboard* keyboard, int key){
    keyboard->key_press[key] = true;
}
void key_up(struct Keyboard* keyboard, int key){
    keyboard->key_press[key] = false;
}
bool is_key_down(struct Keyboard* keyboard, int key){
    return keyboard->key_press[key];
}