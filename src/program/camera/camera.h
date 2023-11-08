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
void view_mesh(Mesh* mesh, Camera* camera, Mesh* viewed_mesh);
void view_triangle(Triangle* tri, Camera* camera, Triangle* viewed_tri);

#endif