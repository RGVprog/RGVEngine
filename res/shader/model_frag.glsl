#version 460 core
out vec4 FragColor;
in vec2 TexCoord;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;
void main(){    
    FragColor = vec4(1.0f) * texture(material.texture_diffuse1, TexCoord);
}
