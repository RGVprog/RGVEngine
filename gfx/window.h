#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include "../include/rgvengine.h"
#include "rgvmath.h"

struct window {
    GLFWwindow *id;
};

struct window win_init (const u32 width, const u32 height, const char *title);
bool win_should_close(struct window *win);
void win_update (struct window *win);
void win_events (struct window *win, struct camera *cam);
void win_destroy (struct window *win);

#endif
