#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <SDL2/SDL.h>
#include "../camera/camera.h"
#include "../mesh/mesh.h"
#define RGBA 4
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

typedef struct Graphics
{
    double Fov;
    double Z_Near;
    double Z_Far;
    double Aspect_Ratio;
    double Fov_Rad;
    int bg_color[RGBA];
    int line_color[RGBA];
} Graphics;

void set_default_graphics(Graphics* graphics);
Graphics* create_graphics();
void destroy_graphics(Graphics* graphics);
void render_frame(SDL_Window *window, SDL_Renderer *renderer, Graphics* graphics, Camera* camera, Mesh* mesh, Mesh* viewed_mesh);
int trig_compare(const void* a, const void* b);
int insert_trig_compare(Triangle* t1, Triangle* t2);


#endif