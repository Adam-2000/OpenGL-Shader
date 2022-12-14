#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; //顶点颜色
layout (location = 2) in vec2 aTexCoord; //顶点的UV坐标

out vec3 ourPosition;
out vec3 ourColor; //输出颜色
out vec2 TexCoord; //输出顶点UV坐标
uniform float YOffset; //声明一个float 变量
uniform mat4 model; //世界空间矩阵
uniform mat4 view; //摄像机矩阵
uniform mat4 projection; //投影矩阵

void main()
{
    //赋值
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0); //顶点由局部空间变换到裁剪空间
    ourColor = aColor; //顶点颜色赋值
    TexCoord = vec2(aTexCoord.x, aTexCoord.y); //顶点的UV坐标 
}