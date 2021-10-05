#include "screen.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
void screen_set(struct Screen* s, int x, int y){
    s->pixels[y][x] = true;
}
bool screen_is_set(struct Screen* s, int x, int y){
    return s->pixels[y][x];
}
struct Screen* initScreen(){
    struct Screen* sc = (struct Screen*)malloc(sizeof(struct Screen));
    int x, y;
    for(x = 0; x<64; x++)
        for(y = 0; y<32; y++)
            sc->pixels[y][x] = false;
    return sc;
}
bool screen_draw_sprite(struct Screen* s, int x, int y, unsigned const char* sprite, int n){
    bool collision = false;
    unsigned char temp;
    for(int ly = 0; ly<n; ly++){
        temp = sprite[ly];
        for(int lx = 0; lx<8; lx++){
            if((temp & (0b10000000 >> lx)) == 0) continue;
            if(s->pixels[(y+ly)%32][(x+lx)%64])          collision = true;
            s->pixels[(y + ly)%32][(x + lx)%64] = s->pixels[(y + ly)%32][(x + lx)%64] ^ 1;
        }
    }
    return collision;

}