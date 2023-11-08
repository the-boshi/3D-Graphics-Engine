#include "camera.h"
#include <SDL2/SDL.h>

#define POSITION_SIZE 4
#define DIR_VEC_SIZE 4
#define VEL_VEC_SIZE 3

#define DEFAULT_V 0.1
#define DEFAULT_ANGLE_V 0.04

void initialize_camera(Camera *camera)
{
    for (int i = 0; i < POSITION_SIZE; i++)
    {
        camera->position->data[i] = 0;
    }

    for (int i = 0; i < VEL_VEC_SIZE; i++)
    {
        camera->velocity->data[i] = DEFAULT_V;
    }

    double dir_arr[DIR_VEC_SIZE] = { 0, 0, 1, 1 };
    arr_to_vec(camera->direction, &dir_arr[0], VECTOR_4D_SIZE);
}

Camera* create_camera(void)
{
    Camera* camera = (Camera*)malloc(sizeof(Camera));
    camera->position = create_vec(POSITION_SIZE);
    camera->velocity = create_vec(VEL_VEC_SIZE);
    camera->direction = create_vec(DIR_VEC_SIZE);
    initialize_camera(camera);

    return camera;
}

void generate_looks_at_mat(Vector* pos, Vector* target, Vector* up, Matrix* look_at_mat)
{
    Vector* target3d = create_vec(COORD_3D_DIMS);
    copy_vec(target, target3d);

    Vector* pos3d = create_vec(COORD_3D_DIMS);
    copy_vec(pos, pos3d);

    Vector* new_forward = create_vec(COORD_3D_DIMS);
    vec_sub(target3d, pos3d, new_forward);
    //copy_vec(target3d,new_forward);
    vec_normalize(new_forward, new_forward);

    Vector* tmp = create_vec(COORD_3D_DIMS);
    vec_mul_num(up, dot_product(up, new_forward), tmp);

    Vector* new_up = create_vec(COORD_3D_DIMS);
    vec_sub(up, tmp, new_up);
    vec_normalize(new_up, new_up);

    Vector* new_right = create_vec(COORD_3D_DIMS);
    cross_product(new_up, new_forward, new_right);

    double look_at_arr[VECTOR_4D_SIZE][VECTOR_4D_SIZE] = {
        { index_vec(new_right, X_AXIS), index_vec(new_up, X_AXIS), index_vec(new_forward, X_AXIS), 0 },
        { index_vec(new_right, Y_AXIS), index_vec(new_up, Y_AXIS), index_vec(new_forward, Y_AXIS), 0 },
        { index_vec(new_right, Z_AXIS), index_vec(new_up, Z_AXIS), index_vec(new_forward, Z_AXIS), 0 },
        { -dot_product(pos3d, new_right), -dot_product(pos3d, new_up), -dot_product(pos3d, new_forward), 1 }
    };

    arr_to_mat(look_at_mat, &look_at_arr[0][0]);
    

    destroy_vec(target3d); destroy_vec(pos3d); destroy_vec(new_forward); destroy_vec(tmp); destroy_vec(new_up); destroy_vec(new_right); 
}

void view_vector(Vector* vec, Camera* camera, Vector* viewed_vec)
{
    Vector* up = create_vec(COORD_3D_DIMS);
    Vector* target = create_vec(VECTOR_4D_SIZE);

    double arr_up[COORD_3D_DIMS] = { 0, 1, 0 };
    arr_to_vec(up, &arr_up[0], COORD_3D_DIMS);

    double arr_target[VECTOR_4D_SIZE] = { 0, 0, 1, 1 };
    arr_to_vec(target, &arr_target[0], VECTOR_4D_SIZE);



    vec_add(camera->position, camera->direction, target);

    Matrix* look_at_mat = create_mat(VECTOR_4D_SIZE, VECTOR_4D_SIZE);
    generate_looks_at_mat(camera->position, target, up, look_at_mat);

    vec_mat_mul(look_at_mat, vec, viewed_vec);

    viewed_vec->data[3] = 1;
    


    destroy_vec(up); destroy_vec(target); destroy_mat(look_at_mat);
}

void view_triangle(Triangle* tri, Camera* camera, Triangle* viewed_tri)
{
    for (int point = 0; point < NUM_TRIANGLE_POINTS; point++)
    {
        view_vector(tri->points[point], camera, viewed_tri->points[point]);
    }
}

void view_mesh(Mesh* mesh, Camera* camera, Mesh* viewed_mesh)
{
    for (int triangle = 0; triangle < mesh->num_triangles; triangle++)
    {
        view_triangle(mesh->triangles[triangle], camera, viewed_mesh->triangles[triangle]);
    }
}

void update_camera(Camera *camera, Keyboard* keyboard)
{
    /*
    if (keyboard->keys[SDL_SCANCODE_A]) {
        camera->position->data[Z_AXIS] -= camera->velocity->data[Z_AXIS];
    }

    if (keyboard->keys[SDL_SCANCODE_D]) {
        camera->position->data[Z_AXIS] += camera->velocity->data[Z_AXIS];
    }

    */

    if (keyboard->keys[SDL_SCANCODE_SPACE]) {
        camera->position->data[Y_AXIS] -= DEFAULT_V;
    }

    if (keyboard->keys[SDL_SCANCODE_LSHIFT]) {
        camera->position->data[Y_AXIS] += DEFAULT_V;
    }

    if (keyboard->keys[SDL_SCANCODE_W]) {
        Vector* forward = create_vec(VECTOR_4D_SIZE);
        vec_mul_num(camera->direction, DEFAULT_V, forward);
        vec_add(camera->position, forward, camera->position);
        camera->position->data[3] = 1;
        //print_vec(camera->position);
        destroy_vec(forward);
    }

    if (keyboard->keys[SDL_SCANCODE_S]) {
        Vector* forward = create_vec(VECTOR_4D_SIZE);
        vec_mul_num(camera->direction, DEFAULT_V, forward);
        vec_sub(camera->position, forward, camera->position);
        camera->position->data[3] = 1;
        //print_vec(camera->position);
        destroy_vec(forward);
    }

    /*
    if (keyboard->keys[SDL_SCANCODE_UP]) {
        double angles[COORD_3D_DIMS] = { 0, 0, DEFAULT_ANGLE_V }; 
        Matrix* rotation_mat = create_mat(VECTOR_4D_SIZE, VECTOR_4D_SIZE);
        generate_rotatation_mat(rotation_mat, angles);
        mat_vec_mul(rotation_mat, camera->direction, camera->direction);
        destroy_mat(rotation_mat);
    }

    if (keyboard->keys[SDL_SCANCODE_DOWN]) {
        double angles[COORD_3D_DIMS] = { 0, 0, -DEFAULT_ANGLE_V }; 
        Matrix* rotation_mat = create_mat(VECTOR_4D_SIZE, VECTOR_4D_SIZE);
        generate_rotatation_mat(rotation_mat, angles);
        mat_vec_mul(rotation_mat, camera->direction, camera->direction);
        destroy_mat(rotation_mat);
    }
    */

    if (keyboard->keys[SDL_SCANCODE_D]) {
        //print_vec(camera->direction);
        double angles[COORD_3D_DIMS] = { 0, DEFAULT_ANGLE_V, 0 }; 
        Matrix* rotation_mat = create_mat(VECTOR_4D_SIZE, VECTOR_4D_SIZE);
        generate_rotatation_mat(rotation_mat, angles);
        mat_vec_mul(rotation_mat, camera->direction, camera->direction);
        destroy_mat(rotation_mat);
    }

    if (keyboard->keys[SDL_SCANCODE_A]) {
        //print_vec(camera->direction);
        double angles[COORD_3D_DIMS] = { 0, -DEFAULT_ANGLE_V, 0 }; 
        Matrix* rotation_mat = create_mat(VECTOR_4D_SIZE, VECTOR_4D_SIZE);
        generate_rotatation_mat(rotation_mat, angles);
        mat_vec_mul(rotation_mat, camera->direction, camera->direction);
        destroy_mat(rotation_mat);
    }
}

void destroy_camera(Camera *camera)
{
    destroy_vec(camera->velocity);
    destroy_vec(camera->position);
    free(camera);
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
    Camera* camera = create_camera();
    print_vec(camera->position);

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
        update_camera(camera, keyboard);

        print_vec(camera->position);
        printf("\n");
        
        SDL_Delay(time_left());
        next_time += TICK_INTERVAL;
    }

    destroy_keyboard(keyboard);
    SDL_Quit();
}
*/

