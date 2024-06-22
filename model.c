#include "gfx/model.h"
#include "gfx/mesh.h"
#include "gfx/shader.h"
#include "include/d_array.h"
#include "include/rgvengine.h"

#include <assimp/cimport.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/types.h>
#include <stdio.h>
#include <string.h>

void substr (char *s, i32 a, i32 b, char *t){
    strncpy(t, s+a, b);
}

i32 find_last_of (char c, char *s){
    for (i32 i = strlen(s) - 1; i >= 0; i--){
        if(c == s[i]) return i;
    }

    printf("ERROR :: FAIL IN FINDING LAST OF\n");
    return 0;
}

d_array load_material_textures (struct model *modl, struct aiMaterial *mat, enum aiTextureType type, char *typename){
    d_array textures = d_array_create(sizeof(struct texture), 0);
    for (u32 i = 0; i < aiGetMaterialTextureCount(mat, type); i++){
        struct aiString str;
        aiGetMaterialTexture(mat, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);
        bool skip = false;
        
        for (u32 j = 0; j < modl->textures_loaded.len; j++){
            if(strcmp(d_array_at(struct texture, j, modl->textures_loaded.ptr).path, str.data) == 0){
                d_array_push(textures, struct texture, d_array_at(struct texture, j, modl->textures_loaded.ptr));
                skip = true;
                break;
            }
        }

        if(!skip){
            struct texture tex;
            tex.id = texture_load(str.data, modl->directory);
            strcpy(tex.type, typename);
            strcpy(tex.path, str.data);
            d_array_push(textures, struct texture, tex);
            d_array_push(modl->textures_loaded, struct texture, tex);
        }
    }

    return textures;
}

struct mesh mesh_process(struct model *modl, struct aiMesh *mesh, const struct aiScene *scene){
    d_array vertices = d_array_create(sizeof(struct vertex), 0);
    d_array indices = d_array_create(sizeof(u32), 0);
    d_array textures = d_array_create(sizeof(struct texture), 0);

    for(u32 i = 0; i < mesh->mNumVertices; i++){
        vec2 myvec;

        if(mesh->mTextureCoords[0]){
            myvec[0] = mesh->mTextureCoords[0][i].x;
            myvec[1] = mesh->mTextureCoords[0][i].y;
        }
        else{
            myvec[0] = 0.0f;
            myvec[1] = 0.0f;
        }
    
        struct vertex vert = {
            .pos = {
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z,
            },

            .norm = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z,
            },

            .texCoord = {
                myvec[0], myvec[1],
            },
        };
        d_array_push(vertices, struct vertex, vert);
    }

    for(u32 i = 0; i < mesh->mNumFaces; i++){
        struct aiFace face = mesh->mFaces[i];
        for (u32 j = 0; j < face.mNumIndices; j++)
            d_array_push(indices, u32, face.mIndices[j]);
    }

    if(mesh->mMaterialIndex >= 0){
        struct aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
        d_array diffMaps = load_material_textures(modl, mat, aiTextureType_DIFFUSE, "texture_diffuse");
        for(u32 i = 0; i < diffMaps.len; i++)
            d_array_push(textures, struct texture, d_array_at(struct texture, i, diffMaps.ptr));

        d_array specMaps = load_material_textures(modl, mat, aiTextureType_SPECULAR, "texture_specular");
        for(u32 i = 0; i < specMaps.len; i++)
            d_array_push(textures, struct texture, d_array_at(struct texture, i, specMaps.ptr));
    }

    return mesh_init(vertices, indices, textures);

}

void node_process(struct model *modl, struct aiNode *node, const struct aiScene *scene){
    for (u32  i = 0; i < node->mNumMeshes; i++){
        struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        d_array_push(modl->meshes, struct mesh, mesh_process(modl, mesh, scene));
    }

    for (u32 i = 0; i < node->mNumChildren; i++)
        node_process(modl, node->mChildren[i], scene);
}

void model_load (char *path, struct model *modl) {
    const struct aiScene *scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){ printf("ERROR :: ASSIMP %s\n", aiGetErrorString());exit(1);};
    
    strncpy(modl->directory, path, find_last_of('/', path));
    node_process(modl, scene->mRootNode, scene);
}

struct model model_init(char *path){
    struct model modl;
    modl.meshes = d_array_create(sizeof(struct model), 0);
    modl.textures_loaded = d_array_create(sizeof(struct texture), 0);
    model_load(path, &modl);
    return modl;
}

void model_draw(struct model *modl, struct shader *shdr){
    for (u32 i = 0; i < modl->meshes.len; i++)
        mesh_draw(&d_array_at(struct mesh, i, modl->meshes.ptr), shdr);
}

void model_delete(struct model *modl){
    for(u32 i = 0; i < modl->meshes.len; i++){
        mesh_delete(&d_array_at(struct mesh, i, modl->meshes.ptr));
    }

    d_array_free(&modl->meshes);
    d_array_free(&modl->textures_loaded);
}
