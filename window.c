#define GLFW_INCLUDE_NONE

#include "gfx/rgvmath.h"

#include "include/rgvengine.h"
#include "gfx/window.h"

#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <stdbool.h>

void frame_buffer_size_callback (GLFWwindow *window, i32 width, i32 height){
    glViewport(0, 0, width, height);
}

struct window win_init(const u32 width, const u32 height, const char *title){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *win_id = glfwCreateWindow(width, height, title, NULL, NULL);
    ASSERT(win_id != NULL, "ERROR :: FAILURE CREATING WINDOW");

    glfwMakeContextCurrent(win_id);
    glfwSetFramebufferSizeCallback(win_id, frame_buffer_size_callback);
    glfwSetCursorPosCallback(win_id, mouse_callback);

    glfwSetInputMode(win_id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "ERROR :: FAILURE INITITALIZING GLAD");
    
    glEnable(GL_DEPTH_TEST);
    
    return (struct window){ .id = win_id };
}

bool win_should_close(struct window *win) {
    return glfwWindowShouldClose(win->id);
}

void win_update(struct window *win){
    glfwSwapBuffers(win->id);
    glfwPollEvents();
}

f32 deltaTime = 0.0f;
f32 lastFrame = 0.0f;

void win_events(struct window *win, struct camera *cam){

    f32 currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if(glfwGetKey(win->id, GLFW_KEY_F2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(glfwGetKey(win->id, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetInputMode(win->id, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else if(glfwGetKey(win->id, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        glfwSetInputMode(win->id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(glfwGetKey(win->id, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam->speed = 8.0f * deltaTime;
    else
        cam->speed = 4.0f * deltaTime;

    if(glfwGetKey(win->id, GLFW_KEY_W) == GLFW_PRESS)
        camera_forward(cam);

    if(glfwGetKey(win->id, GLFW_KEY_S) == GLFW_PRESS)
        camera_back(cam);

    if(glfwGetKey(win->id, GLFW_KEY_A) == GLFW_PRESS)
        camera_left(cam);

    if(glfwGetKey(win->id, GLFW_KEY_D) == GLFW_PRESS)
        camera_right(cam);

}

void win_destroy(struct window *win){
    glfwDestroyWindow(win->id);
}
