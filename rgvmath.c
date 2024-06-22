#define GLFW_INCLUDE_NONE

#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/util.h>
#include <cglm/vec3.h>
#include <math.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>

#include "gfx/rgvmath.h"
#include "gfx/shader.h"
#include "include/rgvengine.h"

mat4 VIEW;
mat4 PROJ;
mat4 MODL;

vec3 DIR = {0.0f, 0.0f, -1.0f};
bool FIRST_MOUSE;
f32 lastX; 
f32 lastY;
f32 SENSITIVITY;
f32 YAW = -90.0f;
f32 PITCH = 0.0f;

void mouse_callback(GLFWwindow *win, f64 xposIn, f64 yposIn){
    f32 xpos = (f32)xposIn;
    f32 ypos = (f32)yposIn;

    if(FIRST_MOUSE){
        lastX = xpos;
        lastY = ypos;
        FIRST_MOUSE = false;
    }

    f32 xoffset = xpos - lastX;
    f32 yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;

    YAW += xoffset;
    PITCH += yoffset;

    if(PITCH > 89.0f)
        PITCH = 89.0f;
    if(PITCH < -89.0f)
        PITCH = -89.0f;

    DIR[0] = cos(glm_rad(YAW)) * cos(glm_rad(PITCH));
    DIR[1] = sin(glm_rad(PITCH));
    DIR[2] = sin(glm_rad(YAW)) * cos(glm_rad(PITCH));
}

void rgv_tranform_init(const f32 fovy, const f32 scr_width, const f32 scr_height, const f32 sens) {
    glm_mat4_identity(PROJ);
    glm_perspective(glm_rad(fovy), scr_width / scr_height, 0.1f, 100.0f, PROJ);

    glm_mat4_identity(VIEW);
    glm_translate(VIEW, (vec3){0.0f, 0.0f, -3.0f});

    glm_mat4_identity(MODL);

    lastX = scr_width/2.0f;
    lastY = scr_height/2.0f;

    SENSITIVITY = sens;
}


void rgv_transform_update(const struct shader *s, rgv_model_manipulations manipulations_func) {
    glm_mat4_identity(MODL);
    manipulations_func(MODL);

    shader_set_mat4(s, MODL, "model");
    shader_set_mat4(s, VIEW, "view");
    shader_set_mat4(s, PROJ, "proj");
}

struct camera camera_init() {
    struct camera cam = {
        .pos = {0.0f, 0.0f, 3.0f},
        .front = {DIR[0], DIR[1], DIR[2]},
        .up = {0.0f, 1.0f, 0.0f},

        .speed = 0.05f,
    };

    return cam;
}

void camera_update(struct camera *cam){
    cam->front[0] = DIR[0];
    cam->front[1] = DIR[1];
    cam->front[2] = DIR[2];

    vec3 temp;
    glm_vec3_add(cam->pos, cam->front, temp);
    glm_lookat(cam->pos, temp, cam->up, VIEW);
}

void camera_forward(struct camera *cam){
    vec3 temp;
    glm_vec3_scale(cam->front, cam->speed, temp);
    glm_vec3_add(cam->pos, temp, cam->pos);
}

void camera_back(struct camera *cam){
    vec3 temp;
    glm_vec3_scale(cam->front, cam->speed, temp);
    glm_vec3_sub(cam->pos, temp, cam->pos);
}

void camera_left(struct camera *cam) {
    vec3 temp;
    glm_vec3_cross(cam->front, cam->up, temp);
    glm_vec3_normalize(temp);
    glm_vec3_scale(temp, cam->speed, temp);
    glm_vec3_sub(cam->pos, temp, cam->pos);
}

void camera_right(struct camera *cam) {
    vec3 temp;
    glm_vec3_cross(cam->front, cam->up, temp);
    glm_vec3_normalize(temp);
    glm_vec3_scale(temp, cam->speed, temp);
    glm_vec3_add(cam->pos, temp, cam->pos);
}
