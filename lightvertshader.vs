#version 330 core
layout (location = 0) in vec3 aPos; //顶点位置

uniform mat4 model;                //世界空间矩阵
uniform mat4 view;                //摄像机矩阵
uniform mat4 projection;         //投影矩阵

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

}