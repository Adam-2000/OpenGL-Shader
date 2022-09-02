#version 330 core
out vec4 FragColor;

in vec3 ourPosition;
in vec2 TexCoord;
uniform vec4 outColor;
uniform sampler2D texture3; //声明一个贴图

void main()
{
    
    vec4 tex = texture(texture3, TexCoord) * outColor;
    FragColor = tex;
}