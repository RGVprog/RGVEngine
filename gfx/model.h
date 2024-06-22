#ifndef MODEL_H
#define MODEL_H

#include "../include/rgvengine.h"
#include "../gfx/shader.h"
#include "../include/d_array.h"

struct model {
    d_array meshes;
    d_array textures_loaded;
    char directory[1024];
};

struct model model_init(char *path);
void model_draw(struct model *modl, struct shader *shdr);
void model_delete(struct model *modl);

#endif
