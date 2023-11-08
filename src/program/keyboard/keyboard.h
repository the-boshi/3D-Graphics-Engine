#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdbool.h>
#include <SDL2/SDL.h>

#define NUM_KEYS 

typedef struct Keyboard
{
    bool keys[SDL_NUM_SCANCODES];
} Keyboard;

Keyboard* create_keyboard(void);
void initialize_keyboard(Keyboard *keyboard);
void update_keyboard(Keyboard *keyboard, SDL_Event e);
void destroy_keyboard(Keyboard *keyboard);
void print_keyboard(Keyboard* keyboard);

#endif