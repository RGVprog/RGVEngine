#include "include/d_array.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include "include/rgvengine.h"
#include "gfx/mesh.h"
#include "gfx/shader.h"

#include "include/glad/glad.h"
#include "include/stb_image.h"

struct mesh mesh_init(d_array vertices, d_array indices, d_array textures){

    u32 p_vao, p_vbo, p_ebo;

    glGenVertexArrays(1, &p_vao);
    glGenBuffers(1, &p_vbo);
    glGenBuffers(1, &p_ebo);

    glBindVertexArray(p_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size, vertices.ptr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size, indices.ptr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *)(offsetof(struct vertex, norm)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *)(offsetof(struct vertex, texCoord)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return (struct mesh) {
        .vertices = vertices,
        .indices = indices,
        .textures = textures,

        .VAO = p_vao,
        .VBO = p_vbo,
        .EBO = p_ebo,
    };
}

u32 texture_load(const char *path, const char *directory){

    char filename[512];
    char temp[512];

    for (u32 i = strlen(path); i >= 2; --i){
      temp[i - 2] = path[i];
    }
    sprintf(filename, "%s/%s", directory, path);

    i32 width, height, num_Channels;
    u32 id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &num_Channels, 0);

    ASSERT(data, "ERROR :: FAILURE LOADING TEXTURES\n");

    if(data){

        GLenum flag;
        switch (num_Channels) {
            case 1: flag = GL_RED; break;
            case 3: flag = GL_RGB; break;
            case 4: flag = GL_RGBA; break;
        }
 
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, flag, width, height, 0, flag, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }

    stbi_image_free(data);

    return id;
}

void mesh_draw(struct mesh *mesh, struct shader *shader){
    u32 num_diff = 1;
    u32 num_spec = 1;
    for(u32 i = 0; i < mesh->textures.len; i++){
        glActiveTexture(GL_TEXTURE0 + i);    
    
        char num [32];
        char name [32];
        strcpy(name, d_array_at(struct texture, i, mesh->textures.ptr).type);

        if(strcmp(name, "texture_diffuse") == 0)
            sprintf(num, "%d", num_diff++);
        else if(strcmp(name, "texture_specular") == 0)
            sprintf(num, "%d", num_spec++);
   
        char temp[64];
        sprintf(temp, "material.%s%s", name, num);
        shader_set_i32(shader, i, temp);
        glBindTexture(GL_TEXTURE_2D, d_array_at(struct texture, i, mesh->textures.ptr).id);
    }

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->indices.len, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void mesh_delete(struct mesh *mesh) {
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);

    d_array_free(&mesh->vertices);
    d_array_free(&mesh->indices);
    d_array_free(&mesh->textures);
}
