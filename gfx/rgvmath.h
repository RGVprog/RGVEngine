#ifndef RGVMATH_H
#define RGVMATH_H

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "../include/rgvengine.h"
#include "shader.h"

struct camera {
    vec3 pos;
    vec3 front;
    vec3 up;

    f32 speed;
};

void rgv_tranform_init(const f32 fovy, const f32 width, const f32 height, const f32 sens);
void rgv_transform_update(const struct shader *s, rgv_model_manipulations);
void mouse_callback(GLFWwindow *win, f64 xposIn, f64 yposIn);

struct camera camera_init();
void camera_update(struct camera *cam);
void camera_forward(struct camera *cam);
void camera_back(struct camera *cam);
void camera_left(struct camera *cam);
void camera_right(struct camera *cam);

#endif
