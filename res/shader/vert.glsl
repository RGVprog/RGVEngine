#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main () {
    Normal = vNormal;
    TexCoord = vTexCoord;
    FragPos = vec3(model * vec4(vPos, 1.0f));
    gl_Position = proj * view * model * vec4(vPos, 1.0f);
}
