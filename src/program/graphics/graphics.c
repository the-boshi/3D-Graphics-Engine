#include "graphics.h"
#include <stdbool.h>

#define WHITE { 255, 255, 255, 255 }
#define BLACK { 0, 0, 0, 255 }
#define PINK { 255, 0, 100, 255 }
#define MIN_SHADE 0.1

void render_frame(SDL_Window *window, SDL_Renderer *renderer, Graphics* graphics, Camera* camera, Mesh* mesh, Mesh* viewed_mesh);
void project_triangle(Triangle* triangle, Matrix* mat_proj, Triangle* tri_projected);
void draw_triangle(SDL_Window *window, SDL_Renderer *renderer, Triangle* triangle, int color[4]);
void project_vec(Vector* vec3, Matrix* mat_proj, Vector* result);
void draw_line(SDL_Window *window, SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int color[4]);
bool point_in_frame(int x, int y);
void scale_triangle(Triangle* triangle);
void fill_triangle(SDL_Window *window, SDL_Renderer *renderer, Triangle* triangle, int color[4]);
void render_triangle(SDL_Window *window, SDL_Renderer *renderer, Graphics* graphics, Triangle* triangle, Camera* camera, Matrix* mat_proj);

void set_default_graphics(Graphics* graphics)
{
    graphics->Fov = 90;
    graphics->Z_Near = 0.1;
    graphics->Z_Far = 1000;
    graphics->Aspect_Ratio = WINDOW_HEIGHT/WINDOW_WIDTH;
    graphics->Fov_Rad = 1 / tan(graphics->Fov * 0.5 * PI / 180);
    
    int bg_color[RGBA] = BLACK;
    int line_color[RGBA] = WHITE;

    for (int i = 0; i < RGBA; i++)
    {
        graphics->bg_color[i] = bg_color[i];
        graphics->line_color[i] = line_color[i];
    }
}

Graphics* create_graphics()
{
    Graphics* graphics = (Graphics*)malloc(sizeof(Graphics));
    return graphics;
}

void destroy_graphics(Graphics* graphics)
{
    free(graphics);
}

void generate_mat_proj(Matrix* mat_proj, Graphics* graphics)
{
    double arr_proj[4][4] = {
        { graphics->Aspect_Ratio * graphics->Fov_Rad, 0, 0 ,0 },
        { 0, graphics->Fov_Rad, 0, 0},
        { 0, 0, graphics->Z_Far / (graphics->Z_Far - graphics->Z_Near), 1},
        { 0, 0, (-graphics->Z_Far * graphics->Z_Near) / (graphics->Z_Far - graphics->Z_Near), 0},
    };

    arr_to_mat(mat_proj, &arr_proj[0][0]);
}

void draw_background(SDL_Window *window, SDL_Renderer *renderer, Graphics* graphics)
{
    SDL_SetRenderDrawColor(renderer, graphics->bg_color[0], graphics->bg_color[1], graphics->bg_color[2], graphics->bg_color[3]);
    SDL_RenderClear(renderer);
}

double get_triangle_shade(Triangle* t)
{
    Vector* light_dir = create_vec(COORD_3D_DIMS);
    double light_dir_arr[COORD_3D_DIMS] = { -0.7, -0.5, -1};
    arr_to_vec(light_dir, light_dir_arr, COORD_3D_DIMS);
    vec_normalize(light_dir, light_dir);
    double intensity = dot_product(t->normal, light_dir);
    if (intensity < MIN_SHADE)
    {
        intensity = MIN_SHADE;
    }
    //printf("%lf\n", intensity);
    return intensity;
}

bool is_triangle_visible(Triangle* triangle, Camera* camera)
{
    bool is_visible = false;
    Vector* direction = create_vec(COORD_3D_DIMS);
    double dir_arr[COORD_3D_DIMS] = { 0, 0, 1 };
    arr_to_vec(direction, &dir_arr[0], VECTOR_4D_SIZE);
    if (dot_product(triangle->normal, direction) > 0)
    {
        is_visible = true;
    }
    destroy_vec(direction);
    return true;
}


int trig_compare(const void* a, const void* b)
{
    //printf("trig cmpr used!\n");
    
    Triangle* t1 = *((Triangle**) a);
    Triangle* t2 = *((Triangle**) b);

    double z1 = (t1->points[0]->data[2] + t1->points[1]->data[2] + t1->points[2]->data[2]) / 3;
    double z2 = (t2->points[0]->data[2] + t2->points[1]->data[2] + t2->points[2]->data[2]) / 3;
   
    //printf("trig cmpr success!\n");
    if (z1 < z2) return 1;
    if (z1 > z2) return -1;
    return 0;
}


int insert_trig_compare(Triangle* t1, Triangle* t2)
{
    //printf("trig cmpr used!\n");

    double z1 = (t1->points[0]->data[2] + t1->points[1]->data[2] + t1->points[2]->data[2]) / 3;
    double z2 = (t2->points[0]->data[2] + t2->points[1]->data[2] + t2->points[2]->data[2]) / 3;
   
    //printf("trig cmpr success!\n");
    if (z1 < z2) return -1;
    return 0;
}

void sort_mesh(Mesh* m)
{
    qsort(m->triangles, m->num_triangles, sizeof(*(m->triangles)), trig_compare);
}

void insertion_sort_mesh(Mesh* mesh, Mesh* viewed_mesh)
{
    int i, j;
    Triangle* key;
    Triangle* key2;
    for (i = 1; i < mesh->num_triangles; i++) 
    {
        key = viewed_mesh->triangles[i];
        key2 = mesh->triangles[i];
        j = i - 1;
 
        /* Move elements of arr[0..i-1], 
           that are greater than key, 
           to one position ahead of 
           their current position */
        //while (j >= 0 && arr[j] > key) 
        while (j >= 0 && insert_trig_compare(viewed_mesh->triangles[j], key)) 
        {
            viewed_mesh->triangles[j + 1] = viewed_mesh->triangles[j];
            mesh->triangles[j + 1] = mesh->triangles[j];
            j = j - 1;
        }
        viewed_mesh->triangles[j + 1] = key;
        mesh->triangles[j + 1] = key2;
    }
}

void render_mesh(SDL_Window *window, SDL_Renderer *renderer, Graphics* graphics, Camera* camera, Mesh* mesh, Mesh* viewed_mesh)
{
    Matrix* mat_proj = create_mat(4, 4);
    generate_mat_proj(mat_proj, graphics);

    for (int trig_idx = 0; trig_idx < mesh->num_triangles; trig_idx++)
    {
        viewed_mesh->triangles[trig_idx]->shade = get_triangle_shade(mesh->triangles[trig_idx]);
        view_triangle(mesh->triangles[trig_idx], camera, viewed_mesh->triangles[trig_idx]);
    }

    insertion_sort_mesh(mesh, viewed_mesh);

    for (int trig_idx = 0; trig_idx < viewed_mesh->num_triangles; trig_idx++)
    {
        if (is_triangle_visible(viewed_mesh->triangles[trig_idx], camera))
        {
            render_triangle(window, renderer, graphics, viewed_mesh->triangles[trig_idx], camera, mat_proj);
        }
        
    }
    destroy_mat(mat_proj); 
}

void render_triangle(SDL_Window *window, SDL_Renderer *renderer, Graphics* graphics, Triangle* triangle, Camera* camera, Matrix* mat_proj)
{
    int triangle_color[4] = PINK;
    for (int i = 0; i < 3; i++)
    {
        triangle_color[i] *= triangle->shade;
    }
    Triangle* tri_projected = create_triangle();
    project_triangle(triangle, mat_proj, tri_projected);
    scale_triangle(tri_projected);
    fill_triangle(window, renderer, tri_projected, triangle_color);
    //draw_triangle(window, renderer, tri_projected, graphics->line_color);
    destroy_triangle(tri_projected);
}

void render_frame(SDL_Window *window, SDL_Renderer *renderer, Graphics* graphics, Camera* camera, Mesh* mesh, Mesh* viewed_mesh)
{
    draw_background(window, renderer, graphics);
    render_mesh(window, renderer, graphics, camera, mesh, viewed_mesh);
    SDL_RenderPresent(renderer);
}

void scale_triangle(Triangle* triangle)
{
    triangle->points[0]->data[0] += 1;
    triangle->points[0]->data[1] += 1;

    triangle->points[1]->data[0] += 1;
    triangle->points[1]->data[1] += 1;

    triangle->points[2]->data[0] += 1;
    triangle->points[2]->data[1] += 1;

    triangle->points[0]->data[0] *= 0.5 * WINDOW_WIDTH;
    triangle->points[0]->data[1] *= 0.5 * WINDOW_HEIGHT;

    triangle->points[1]->data[0] *= 0.5 * WINDOW_WIDTH;
    triangle->points[1]->data[1] *= 0.5 * WINDOW_HEIGHT;

    triangle->points[2]->data[0] *= 0.5 * WINDOW_WIDTH;
    triangle->points[2]->data[1] *= 0.5 * WINDOW_HEIGHT;
}

void project_triangle(Triangle* triangle, Matrix* mat_proj, Triangle* tri_projected)
{
    for (int i = 0; i < NUM_TRIANGLE_POINTS; i++)
    {
        project_vec(triangle->points[i], mat_proj, tri_projected->points[i]);
    }

}

void fill_triangle(SDL_Window *window, SDL_Renderer *renderer, Triangle* triangle, int color[4])
{
    int x1 = (int)triangle->points[0]->data[0];
    int y1 = (int)triangle->points[0]->data[1];

    int x2 = (int)triangle->points[1]->data[0];
    int y2 = (int)triangle->points[1]->data[1];

    int x3 = (int)triangle->points[2]->data[0];
    int y3 = (int)triangle->points[2]->data[1];

    SDL_Vertex triangle_vertex[3] = 
    {
        {
            { x1, y1 }, { color[0], color[1], color[2], 0xFF }, { 0, 0 }
        },
        {
            { x2, y2 }, { color[0], color[1], color[2], 0xFF }, { 0, 0 }
        },
        {
            { x3, y3 }, { color[0], color[1], color[2], 0xFF }, { 0, 0 }
        },
    };

    if (point_in_frame(x1, y1) && point_in_frame(x2, y2) && point_in_frame(x3, y3))
    {
        SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
        SDL_RenderGeometry(renderer, NULL, triangle_vertex, NUM_TRIANGLE_POINTS, NULL, 0);
    }
}

void draw_triangle(SDL_Window *window, SDL_Renderer *renderer, Triangle* triangle, int color[4])
{
    int x1 = (int)triangle->points[0]->data[0];
    int y1 = (int)triangle->points[0]->data[1];

    int x2 = (int)triangle->points[1]->data[0];
    int y2 = (int)triangle->points[1]->data[1];

    int x3 = (int)triangle->points[2]->data[0];
    int y3 = (int)triangle->points[2]->data[1];

    draw_line(window, renderer, x1, y1, x2, y2, color);
    draw_line(window, renderer, x2, y2, x3, y3, color);
    draw_line(window, renderer, x3, y3, x1, y1, color);
}

void project_vec(Vector* vec, Matrix* mat_proj, Vector* result)
{
    double z = vec->data[2];
    mat_vec_mul(mat_proj, vec, result);
    vec_div(result, z, result);
}

void draw_line(SDL_Window *window, SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int color[4])
{    

    if (point_in_frame(x1, y1) && point_in_frame(x2, y2))
    {
        SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

bool point_in_frame(int x, int y)
{
    return (x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT);
}

void clip_triangles()
{

}