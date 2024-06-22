#define GLFW_INCLUDE_NONE

#include "gfx/model.h"
#include "gfx/shader.h"
#include "include/rgvengine.h"
#include "gfx/window.h"
#include "gfx/rgvmath.h"

#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

void manipulations (mat4 mat) {
    //glm_scale(mat, (vec3){50.0f, 1.0f, 50.0f});
    //glm_scale(mat, (vec3){ sin(glfwGetTime() * 10.0f) * 20.0f, cos(glfwGetTime() * 10.0f) * 20.0f, 1.0f });
    glm_rotate(mat, glm_rad(glfwGetTime() * 10.0f), (vec3){0.0f, 1.0f, 0.0f});
    glm_translate(mat, (vec3){0.0f, 0.0f, 0.0f});
    glm_scale(mat, (vec3){1.0f, 1.0f, 1.0f});
}

void light_manipulations (mat4 mat) {
    glm_translate(mat, (vec3){0.0f, 2.0f, -1.0f});
    glm_scale(mat, (vec3){0.5f, 0.5f, 0.5f});
    //glm_translate(mat, (vec3){0.0f, 2.0f, -1.0f});
}

int main (int argc, char **argv){ 

    struct window engine_window = win_init(SCR_WIDTH, SCR_HEIGHT, "RGV Engine");
    struct shader main_shader = shader_init("res/shader/vert.glsl", "res/shader/frag.glsl");
    struct model my_model = model_init("res/objs/planet/planet.obj");
    struct camera engine_camera = camera_init();

    rgv_tranform_init(90.0f, (f32)SCR_WIDTH, (f32)SCR_HEIGHT, 0.1f);


    while (!win_should_close(&engine_window)) {
        win_update(&engine_window);
        win_events(&engine_window, &engine_camera);

        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera_update(&engine_camera);

        shader_use(&main_shader);
        rgv_transform_update(&main_shader, manipulations);

        shader_set_vec3(&main_shader, (vec3){1.0f, 1.0f, 1.0f}, "lightColor");
        shader_set_vec3(&main_shader, engine_camera.pos, "viewPos");
        shader_set_vec3(&main_shader, (vec3){0.0f, 4.0f, 1.0f}, "light.pos");

        shader_set_vec3(&main_shader, (vec3){1.0, 0.5f, 0.31f}, "material.ambient");
        shader_set_f32(&main_shader, 32.0f, "material.shininess");

        shader_set_vec3(&main_shader, (vec3){0.2f, 0.2f, 0.2f}, "light.ambient");
        shader_set_vec3(&main_shader, (vec3){0.5f, 0.5f, 0.5f}, "light.diffuse");
        shader_set_vec3(&main_shader, (vec3){1.0f, 1.0f, 1.0f}, "light.specular");

        model_draw(&my_model, &main_shader);

    }
    
    model_delete(&my_model);
    shader_delete(&main_shader);
    glfwTerminate();
    return 0;
}
