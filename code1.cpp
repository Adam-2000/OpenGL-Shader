//GLAD��ͷ�ļ���������ȷ��OpenGLͷ�ļ�������GL/gl.h����������Ҫ������������OpenGL��ͷ�ļ�֮ǰ����GLAD��
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//GLM��ѧ��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderBase.h"        //��������Ⱦ��ɫ��
#include "CameraBase.h"       //����������� 
#include "ModelBase.h"        //ģ����

#include <iostream>
//����ǰ������
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

//���ô��ڵĿ�͸�
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//��ʼ�������λ��
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//������Ⱦ��֮֡�� ��ʱ��
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//������
int main()
{
	//��ʼ��GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);         //���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);         //�ΰ汾��  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //����IOSƻ��
#endif

	//�������ڲ��������С�����ƣ������Ƿ񴴽��ɹ�
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//�������֮����Ҫ�õ�ǰ���ڵĻ����ڵ�ǰ�߳��ϳ�Ϊ��ǰ���������ǽ������Ļ�ͼ���ử�����Ǹոմ����Ĵ�����
	glfwMakeContextCurrent(window);
	//����GLFW����ϣ��ÿ�����ڵ�����С��ʱ������������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//��������ƶ��¼� �ص������Ӧ����
	glfwSetCursorPosCallback(window, mouse_callback);
	////���������ֵ��ƶ�
	glfwSetScrollCallback(window, scroll_callback);

	//�������Ĺ�� ����һֱͣ���ڴ�����
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//gladѰ��opengl�ĺ�����ַ������opengl�ĺ���ǰ��Ҫ��ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//������Ȳ��ԣ�Ĭ���ǹرյ�
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//�����ͱ���Shader  ��ȡ��ɫ��·��
	Shader ourShader("nanosuitVS.vs", "nanosuitFS.fs");

	//����ģ��
	Model ourModel(("resources/meshs/nanosuit/nanosuit.obj"));
	//Model ourModel(("resources/meshs/winter-girl/Winter Girl.obj"));
	//Model ourModel(("resources/meshs/scifi-girl/girl_complete_03.obj"));
	// �����߿�
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//��Ⱦѭ��
	if (!glfwWindowShouldClose(window)) {
		//����������֡����Ⱦʱ���
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//��Ӧ��������
		processInput(window);

		//���������ɫ
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		//�����ǰ���ڣ�����ɫ����Ϊ�����ɫ
		//��������Ϣ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//�������ӳ��򣬼�����ɫ������ʼ��Ⱦ
		ourShader.use();

		// �ӿڣ�ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// ģ�Ϳռ�--����ռ����
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", model);
		glm::vec4 lightDir(-0.5, -0.5, 0, 0);
		lightDir = glm::normalize(lightDir);
		glm::vec3 lightColor(1.0, 1.0, 1.0);
		ourShader.setVec4("lightDir1", lightDir);
		ourShader.setVec3("lightColor1", lightColor);
		lightDir = glm::normalize(glm::vec4(0.5, -0.5, 0, 0));
		ourShader.setVec4("lightDir2", lightDir);
		ourShader.setVec3("lightColor2", lightColor);
		lightDir = glm::normalize(glm::vec4(0.2, -0.8, -0.5, 0));
		ourShader.setVec4("lightDir3", lightDir);
		ourShader.setVec3("lightColor3", lightColor);
		ourShader.setVec3("ambientLight", glm::vec3(0.1, 0.1, 0.1));
		ourModel.Draw(ourShader);


		//������ɫ����
		glfwSwapBuffers(window);
		//�����¼�
		glfwPollEvents();
	}
	while (!glfwWindowShouldClose(window))
	{
		//����������֡����Ⱦʱ���
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//��Ӧ��������
		processInput(window);

		//���������ɫ
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		//�����ǰ���ڣ�����ɫ����Ϊ�����ɫ
		//��������Ϣ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//�������ӳ��򣬼�����ɫ������ʼ��Ⱦ
		ourShader.use();

		// �ӿڣ�ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// ģ�Ϳռ�--����ռ����
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", model);

		ourModel.Draw(ourShader);

		//������ɫ����
		glfwSwapBuffers(window);
		//�����¼�
		glfwPollEvents();
	}

	//�ͷ�ǰ����������ڴ�
	glfwTerminate();
	return 0;
}

//��Ӧ���������¼�
void processInput(GLFWwindow* window)
{
	//ESC �˳�����
	//glfwGetKey()�����ж�һ�����Ƿ��¡���һ��������GLFW���ھ�����ڶ���������һ��GLFW����������һ������
	//GLFW_KEY_ESCAPE��ʾEsc�������Esc�������ˣ�glfwGetKey������GLFW_PRESS��ֵΪ1�������򷵻�GLFW_RELEASE��ֵΪ0����
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//WASD�ƶ������
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
}

// ���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�������
//�Դ���ע��һ���ص�����(Callback Function)��������ÿ�δ��ڴ�С��������ʱ�򱻵���
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//OpenGL��Ⱦ���ڵĳߴ��С
	//glViewport����ǰ�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ�
	glViewport(0, 0, width, height);
}

//��Ӧ����¼� �������Ļص�����
//xpos,ypos����λ��
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//���Ϊ��һ�ν��� ��¼��ǰ���λ�� Ϊ��ʼ�����λ��
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//���㵱ǰ֡����һ֡��λ�ò�
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // ��������������� ʵ�ʲ��������������ƶ������ ����Ҳ����������ƶ� ʵ��Ӧ������

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

//��Ӧ������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}