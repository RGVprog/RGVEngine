#include "include/glad/glad.h"

#include "include/rgvengine.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gfx/shader.h"

const char *shader_load(const char* path) {
    FILE *shader_file = fopen(path, "r");
    ASSERT(shader_file != NULL, "ERROR :: FAILURE LOADING SHADER FILE :: (INCORRECT PATH / CORRUPT FILE)");

    fseek(shader_file, 0, SEEK_END);
    i32 len = ftell(shader_file);
    fseek(shader_file, 0, SEEK_SET);

    char *src = malloc(sizeof(char) * (len + 1));

    char c;
    u32 i;
    for (i = 0; (c = fgetc(shader_file)) != EOF; i++)
        src[i] = c;

    src[i] = '\0';

    fclose(shader_file);

    return (const char *)src;
}

void shader_check_err(bool isProg, const u32 id){
    int success;
    char infoLog[1024];

    if(!isProg){
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success) { 
            glGetShaderInfoLog(id, 1024, NULL, infoLog); 
            printf("ERROR :: FAILURE COMPILING SHADER :: :: \n%s\n", infoLog);
        }
    }else{
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(id, 1024, NULL, infoLog);
            printf("ERROR :: FAILURE LINKING SHADER PROGRAM :: :: \n%s\n", infoLog);
        }
    }
}

u32 shader_create(const char *src, const GLenum type){
    u32 id = glCreateShader(type);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    shader_check_err(false, id);

    return id;
}

struct shader shader_init(const char *vert_path, const char *frag_path){
    const char *vert_src = shader_load(vert_path);
    const u32 vert = shader_create(vert_src, GL_VERTEX_SHADER);

    const char *frag_src = shader_load(frag_path);
    const u32 frag = shader_create(frag_src, GL_FRAGMENT_SHADER);

    const u32 s_id = glCreateProgram();
    glAttachShader(s_id, vert);
    glAttachShader(s_id, frag);
    glLinkProgram(s_id);
    shader_check_err(true, s_id);

    glDeleteShader(vert);
    glDeleteShader(frag);

    return (struct shader){ .id = s_id };
}

void shader_use(const struct shader *shad){
    glUseProgram(shad->id);
}

void shader_delete(struct shader *shad){
    glDeleteShader(shad->id);
}

void shader_set_f32(const struct shader *shad, const f32 val, const char* name){
    glUniform1f(glGetUniformLocation(shad->id, name), val);
}

void shader_set_i32(const struct shader *shad, const i32 val, const char *name){
    glUniform1i(glGetUniformLocation(shad->id, name), val);
}

void shader_set_vec2(const struct shader *shad, const float *vec, const char *name){
    glUniform2f(glGetUniformLocation(shad->id, name), vec[0], vec[1]);
}

void shader_set_vec3(const struct shader *shad, const float *vec, const char *name){
    glUniform3f(glGetUniformLocation(shad->id, name), vec[0], vec[1], vec[2]);
}

void shader_set_mat4(const struct shader *shad, const vec4 *mat, const char *name){
    glUniformMatrix4fv(glGetUniformLocation(shad->id, name), 1, GL_FALSE, &mat[0][0]);
}
