#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../keyboard/keyboard.h"
#include "../mesh/mesh.h"
#include "../linalg/linalg.h"

#define MAX_VER_ANGLE PI/2

typedef struct Camera
{
    Vector* position;
    Vector* velocity;
    Vector* direction;
    Vector* rot_vel;
    double yaw;
} Camera;

Camera* create_camera(void);
void update_camera(Camera *camera, Keyboard* keyboard);
void destroy_camera(Camera *camera);
void view_triangle(Triangle* tri, Matrix* look_at_mat, Triangle* viewed_tri);
void generate_looks_at_mat(Vector* pos, Vector* target, Vector* up, Matrix* look_at_mat);

#endif