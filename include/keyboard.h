#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdlib.h>
#include <stdbool.h>
#define KEY_SIZE 0x10
struct Keyboard{
    bool key_press[KEY_SIZE];
    char map[16];
};
struct Keyboard* initKey();
int key_map(struct Keyboard* keyboard, char key);
void key_down(struct Keyboard* keyboard, int key);
void key_up(struct Keyboard* keyboard, int key);
bool is_key_down(struct Keyboard* keyboard, int key);
#endif