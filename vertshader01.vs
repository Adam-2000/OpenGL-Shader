#version 330 core
layout (location = 0) in vec3 aPos;             //顶点位置
layout (location = 1) in vec3 aColor;           //顶点颜色
layout (location = 2) in vec2 aTexCoord;        //顶点的UV坐标
layout (location = 3) in vec3 anormal;          //顶点法线

out vec3 ourColor;                              //输出颜色
out vec2 TexCoord;                              //输出顶点UV坐标
out vec3 Normal;                                //输出法线 
out vec3 WorldPosition;                         //世界空间的顶点位置 
//out vec3 ourPosition;
uniform float YOffset;                          //声明一个float 变量
uniform mat4 model;                             //世界空间矩阵
uniform mat4 view;                              //摄像机矩阵
uniform mat4 projection;                        //投影矩阵

void main()
{
    //赋值
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);   //顶点由局部空间变换到裁剪空间
    ourColor = aColor;                                                             //顶点颜色赋值
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);                                     //顶点的UV坐标 
    Normal = mat3(transpose(inverse(model))) * anormal;                            //转换法线的矩阵 由模型空间到世界空间  
    
    WorldPosition = vec3(model * vec4(aPos,1.0f));                                //顶点从物体空间转换世界空间
    
    
}