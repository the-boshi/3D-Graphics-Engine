#include "keyboard.h"

void initialize_keyboard(Keyboard *keyboard)
{
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        keyboard->keys[i] = false;
    }
}

Keyboard* create_keyboard(void)
{
    Keyboard* keyboard = (Keyboard*)malloc(sizeof(Keyboard));
    initialize_keyboard(keyboard);

    return keyboard;
}

void update_keyboard(Keyboard *keyboard, SDL_Event e)
{
    keyboard->keys[e.key.keysym.scancode] = (e.type == SDL_KEYDOWN);
}

void destroy_keyboard(Keyboard *keyboard)
{
    free(keyboard);
}

void print_keyboard(Keyboard* keyboard)
{
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        if (keyboard->keys[i] == true)
        {
            printf("%d, ", i);
        }
    }
    printf("\n");
}

/*
#define TICK_INTERVAL 1000
static Uint32 next_time;

Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    Keyboard* keyboard = create_keyboard();

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
            {
                update_keyboard(keyboard, e);
            }
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        print_keyboard(keyboard);
        
        SDL_Delay(time_left());
        next_time += TICK_INTERVAL;
    }

    destroy_keyboard(keyboard);
    SDL_Quit();
}

*/