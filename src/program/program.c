#include "program.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TICK_INTERVAL 30
#define NUM_TRIGS 12

#define X_ROT_VEL 0.0
#define Y_ROT_VEL 0.013
#define Z_ROT_VEL 0.0

Uint32 time_left(void);

static Uint32 next_time;

Program* start_program()
{
    SDL_Init(SDL_INIT_VIDEO);
    Program* program = (Program*)malloc(sizeof(Program));
    program->window = SDL_CreateWindow("3D Graphics Engine Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    program->renderer = SDL_CreateRenderer(program->window, -1, SDL_RENDERER_ACCELERATED);
    program->graphics = create_graphics();
    set_default_graphics(program->graphics);
    program->camera = create_camera();
    program->keyboard = create_keyboard();

    return program;
}

Mesh* create_mesh_from_txt(char* file_name)
{
    char file_dir[200];
    char* objects_dir = "src/objects/";
    strcpy(file_dir, objects_dir);
    strcat(file_dir, file_name);

    FILE* fptr;
    fptr = fopen(file_dir, "r");
    if (fptr == NULL)
        exit(EXIT_FAILURE);

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, fptr);
    int num_trigs = strtol(line, NULL, 10);

    Mesh* mesh = create_mesh(num_trigs);
    
    for (int trig = 0; trig < num_trigs; trig++)
    {
        for (int vertex = 0; vertex < NUM_TRIANGLE_POINTS; vertex++)
        {
            Vector* vec = mesh->triangles[trig]->points[vertex];
            read = getline(&line, &len, fptr);
            double x, y, z;
            vec->data[0] = strtof(line, NULL);
            vec->data[1] = strtof(line+9, NULL);
            vec->data[2] = strtof(line+2*9, NULL);
            vec->data[3] = 1;
            
        }
        getline(&line, &len, fptr);
    }

    fclose(fptr);
    if (line) 
    {
        free(line);
    }

    return mesh;
}

void main_loop(Program* program)
{  
    bool quit = false;

    SDL_Window* window = program->window;
    SDL_Renderer* renderer = program->renderer;
    Graphics* graphics = program->graphics;
    Camera* camera = program->camera;
    Keyboard* keyboard = program->keyboard;

    char* object_name = "UtahTeapot.obj";
    Mesh* mesh = create_mesh_from_txt(object_name);

    Mesh* viewed_mesh = create_mesh(mesh->num_triangles);

    Vector* mesh_orientation = create_vec(COORD_3D_DIMS);
    double orient_arr[COORD_3D_DIMS] = { 1, 0, 0 };
    arr_to_vec(mesh_orientation, orient_arr, COORD_3D_DIMS);

    Vector* rotation_vels = create_vec(COORD_3D_DIMS);
    double vels_arr[COORD_3D_DIMS] = { X_ROT_VEL, Y_ROT_VEL, Z_ROT_VEL };
    arr_to_vec(rotation_vels, &vels_arr[0], COORD_3D_DIMS);


    Uint32 out_time = SDL_GetTicks();
    Uint32 tick_sum = 0;
    int frame = 0;
    SDL_Event e;
    while (!quit)
    {
        Uint32 t1 = SDL_GetTicks();
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
        
        rotate_mesh(mesh, mesh_orientation);
        vec_add(mesh_orientation, rotation_vels, mesh_orientation);
        for(int i = 0; i < COORD_3D_DIMS; i++)
        {
            mesh_orientation->data[i] = handle_rads(mesh_orientation->data[i]);
        }

        update_camera(camera, keyboard);
        
        render_frame(window, renderer, graphics, camera, mesh, viewed_mesh);

        SDL_Delay(time_left());
        next_time += TICK_INTERVAL;

        Uint32 t2 = SDL_GetTicks();
        tick_sum += t2 - t1;
        frame++;
        float frame_time = (t2 - t1) ;
        //printf("Current FPS is %d \n", (int)(1000/frame_time));
        //printf("Average FPS is %d \n", (int)(1000/(tick_sum/frame)));
        if (frame > 40)
        {
            frame = 0;
            tick_sum = 0;
        }

    }
   
    destroy_mesh(mesh); destroy_mesh(viewed_mesh);
    destroy_vec(mesh_orientation); destroy_vec(rotation_vels);
}

void quit_program(Program *program)
{
    SDL_DestroyWindow(program->window);
    SDL_DestroyRenderer(program->renderer);
    destroy_graphics(program->graphics);
    destroy_keyboard(program->keyboard);
    destroy_camera(program->camera);
    free(program);
    SDL_Quit();
}

Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}

