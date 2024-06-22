#ifndef SHADER_H
#define SHADER_H

#include "../include/rgvengine.h"

#include <cglm/cglm.h>

struct shader {
    u32 id;
};

struct shader shader_init(const char *vert_path, const char *frag_path);
void shader_use(const struct shader *shad);
void shader_delete(struct shader *shad);
void shader_set_vec3(const struct shader *shad, const vec3 vec, const char *name);
void shader_set_vec2(const struct shader *shad, const vec2 vec, const char *name);
void shader_set_f32(const struct shader *shad, const f32 val, const char *name);
void shader_set_mat4(const struct shader *shad, const mat4 mat, const char *name);
void shader_set_i32(const struct shader *shad, const i32 val, const char *name);

#endif
