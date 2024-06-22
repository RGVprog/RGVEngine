#ifndef MESH_H
#define MESH_H

#include "../include/rgvengine.h"
#include "../include/d_array.h"
#include "shader.h"

struct vertex {
    vec3 pos;
    vec3 norm;
    vec2 texCoord;
};

enum textureType {
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
};

struct texture{
    u32 id;
    char type[32];
    // enum textureType type;
    char path[1024];
};


struct mesh {
    d_array vertices;
    d_array indices;
    d_array textures;

    u32 VAO, VBO, EBO; 
};

struct mesh mesh_init(d_array vertices, d_array indices, d_array textures);
void mesh_draw(struct mesh *mesh, struct shader *shader);
void mesh_delete(struct mesh *mesh);
u32 texture_load(const char *path, const char *directory);

#endif
