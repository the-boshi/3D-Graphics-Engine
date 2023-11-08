#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <SDL2/SDL.h>
#include "keyboard/keyboard.h"
#include "camera/camera.h"
#include "graphics/graphics.h"

typedef struct Program
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    Graphics* graphics;
    Camera* camera;
    Keyboard* keyboard;

} Program;

Program* start_program();
void main_loop(Program* program);
void quit_program(Program *program);

#endif