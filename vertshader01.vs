#version 330 core
layout (location = 0) in vec3 aPos;             //����λ��
layout (location = 1) in vec3 aColor;           //������ɫ
layout (location = 2) in vec2 aTexCoord;        //�����UV����
layout (location = 3) in vec3 anormal;          //���㷨��

out vec3 ourColor;                              //�����ɫ
out vec2 TexCoord;                              //�������UV����
out vec3 Normal;                                //������� 
out vec3 WorldPosition;                         //����ռ�Ķ���λ�� 
//out vec3 ourPosition;
uniform float YOffset;                          //����һ��float ����
uniform mat4 model;                             //����ռ����
uniform mat4 view;                              //���������
uniform mat4 projection;                        //ͶӰ����

void main()
{
    //��ֵ
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);   //�����ɾֲ��ռ�任���ü��ռ�
    ourColor = aColor;                                                             //������ɫ��ֵ
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);                                     //�����UV���� 
    Normal = mat3(transpose(inverse(model))) * anormal;                            //ת�����ߵľ��� ��ģ�Ϳռ䵽����ռ�  
    
    WorldPosition = vec3(model * vec4(aPos,1.0f));                                //���������ռ�ת������ռ�
    
    
}