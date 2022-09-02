#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; //������ɫ
layout (location = 2) in vec2 aTexCoord; //�����UV����

out vec3 ourPosition;
out vec3 ourColor; //�����ɫ
out vec2 TexCoord; //�������UV����
uniform float YOffset; //����һ��float ����
uniform mat4 model; //����ռ����
uniform mat4 view; //���������
uniform mat4 projection; //ͶӰ����

void main()
{
    //��ֵ
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0); //�����ɾֲ��ռ�任���ü��ռ�
    ourColor = aColor; //������ɫ��ֵ
    TexCoord = vec2(aTexCoord.x, aTexCoord.y); //�����UV���� 
}