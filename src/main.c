#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include "chip8.h"
#define MULTIPLIER 10

int main(int argc, char** argv)
{
    if(argc < 2){
        printf("Error in file opening. ");
        return 0;
    }
    const char* filename = argv[1];
    FILE* f = fopen(filename, "rb");
    if(f == NULL)   printf("File cannot be opened. ERROR.");
    fseek(f, 0L, SEEK_END);    
    int fsize = ftell(f);    
    fseek(f, 0L, SEEK_SET);
    unsigned char buffer[fsize];
    fread(buffer, fsize, 1, f);
    // struct chip8 c;
    // set_mem(&(c.memory), 50, 69);
    // printf("%d", get_mem(&(c.memory),50));
    // struct Stack* st = initStack();
    // push_stack(st, 69);
    // push_stack(st, 42);
    // printf("Top: %d\n", top_stack(st));
    // pop_stack(st);
    // printf("Top: %d\n", top_stack(st));
    // struct Keyboard* k = initKey();
    // key_down(k, 0x0f);
    // printf("%d\n", is_key_down(k, 0xf));
    // printf("%d\n", is_key_down(k, 0xe));
    struct Chip8 *chip = initChip8();
    load(chip, buffer, fsize);
    chip->sound = 0;
    chip->delay = 0;
    // screen_draw_sprite(chip->sc, 0, 0, &(chip->memory).memory[0], 5);
    // int temp = 0;
    // scanf("%d", &temp);
    // if(temp)    exec(chip, 0x00E0);
    // screen_draw_sprite(chip->sc, 0, 0, &(chip->memory).memory[5], 5);
        // screen_draw_sprite(chip->sc, 0, 0, &(chip->memory).memory[5], 5);
    // unsigned const char d = 0x00;
    // unsigned const char* ptr = &d;
    // screen_draw_sprite(chip->sc, 0, 0, ptr, 5);
    // screen_draw_sprite(chip->sc, 62, 10, &(chip->memory).memory[0], 5);


    // chip->sound = 230;
    // int count = 0;
    // for(int i = 0; i<32 && count <=76; i = i + 6){
    //     for(int j = 0; j<64; j = j + 5){
    //         count = count + 5;
    //     }
    // }
    // screen_set(chip->sc, 0, 0);
    // screen_set(chip->sc, 2, 0);
    //     screen_set(chip->sc, 3, 0);
    //     screen_set(chip->sc, 5, 0);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        "Chips", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_SHOWN);
    int flag = 0;
    int mapped_key;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                flag = 1;
                break;
            }
            case SDL_KEYDOWN:
            {
                mapped_key = key_map(chip->kb, event.key.keysym.sym);
                if (mapped_key != -1)
                {
                    key_down(chip->kb, mapped_key);
                }
                break;
            }
            case SDL_KEYUP:
            {
                mapped_key = key_map(chip->kb, event.key.keysym.sym);
                if (mapped_key != -1)
                {
                    key_up(chip->kb, mapped_key);
                }
                break;
            }
            }
        }
        if (flag)
            break;
        SDL_SetRenderDrawColor(renderer, 46, 52, 61, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        for (int x = 0; x < 64; x++)
        {
            for (int y = 0; y < 32; y++)
            {
                if (screen_is_set((chip->sc), x, y))
                {
                    SDL_Rect r;
                    r.x = x*MULTIPLIER;
                    r.y = y*MULTIPLIER;
                    r.h = (MULTIPLIER);
                    r.w = (MULTIPLIER);
                    SDL_RenderFillRect(renderer, &r);

                }
            }
        }
        if((chip->delay) > 0){ Sleep(1);   (chip->delay)--;}
        if((chip->sound) > 0){    
             Beep(18000, 10*chip->sound);
            chip->sound = 0; 
        }
        unsigned short temp = get_opcode(&(chip->memory), chip->PC);
        // chip->PC = chip->PC + 2;
        exec(chip, temp);

        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    return 0;
}