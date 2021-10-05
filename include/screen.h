#ifndef SCREEN
#define SCREEN
#include <stdbool.h>
#include <stdint.h>


struct Screen{
    bool pixels[32][64];
};
struct Screen* initScreen();
void screen_set(struct Screen* s, int x, int y);
bool screen_is_set(struct Screen* s, int x, int y);
bool screen_draw_sprite(struct Screen* s, int x, int y, unsigned const char* sprite, int n);
#endif