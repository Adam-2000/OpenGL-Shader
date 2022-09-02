//GLAD的头文件包含了正确的OpenGL头文件（例如GL/gl.h），所以需要在其它依赖于OpenGL的头文件之前包含GLAD。
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//GLM数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderBase.h"        //基础的渲染着色器
#include "CameraBase.h"       //基础摄像机类 
#include "ModelBase.h"        //模型类

#include <iostream>
//函数前置声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

//设置窗口的宽和高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//初始化相机的位置
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//缓冲渲染两帧之差 的时间
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//主函数
int main()
{
	//初始化GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);         //主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);         //次版本号  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //适配IOS苹果
#endif

	//创建窗口并设置其大小，名称，与检测是否创建成功
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//创建完毕之后，需要让当前窗口的环境在当前线程上成为当前环境，就是接下来的画图都会画在我们刚刚创建的窗口上
	glfwMakeContextCurrent(window);
	//告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//监听鼠标移动事件 回调鼠标响应函数
	glfwSetCursorPosCallback(window, mouse_callback);
	////监听鼠标滚轮的移动
	glfwSetScrollCallback(window, scroll_callback);

	//隐藏鼠标的光标 光标会一直停留在窗口中
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glad寻找opengl的函数地址，调用opengl的函数前需要初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//启用深度测试，默认是关闭的
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//构建和编译Shader  读取着色器路径
	Shader ourShader("nanosuitVS.vs", "nanosuitFS.fs");

	//导入模型
	Model ourModel(("resources/meshs/nanosuit/nanosuit.obj"));
	//Model ourModel(("resources/meshs/winter-girl/Winter Girl.obj"));
	//Model ourModel(("resources/meshs/scifi-girl/girl_complete_03.obj"));
	// 绘制线框
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//渲染循环
	if (!glfwWindowShouldClose(window)) {
		//计算上下两帧的渲染时间差
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//响应键盘输入
		processInput(window);

		//设置清除颜色
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		//清除当前窗口，把颜色设置为清除颜色
		//清除深度信息
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//激活链接程序，激活着色器，开始渲染
		ourShader.use();

		// 视口，投影矩阵
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// 模型空间--世界空间矩阵
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


		//交换颜色缓冲
		glfwSwapBuffers(window);
		//处理事件
		glfwPollEvents();
	}
	while (!glfwWindowShouldClose(window))
	{
		//计算上下两帧的渲染时间差
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//响应键盘输入
		processInput(window);

		//设置清除颜色
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		//清除当前窗口，把颜色设置为清除颜色
		//清除深度信息
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//激活链接程序，激活着色器，开始渲染
		ourShader.use();

		// 视口，投影矩阵
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// 模型空间--世界空间矩阵
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", model);

		ourModel.Draw(ourShader);

		//交换颜色缓冲
		glfwSwapBuffers(window);
		//处理事件
		glfwPollEvents();
	}

	//释放前面所申请的内存
	glfwTerminate();
	return 0;
}

//响应键盘输入事件
void processInput(GLFWwindow* window)
{
	//ESC 退出窗口
	//glfwGetKey()用来判断一个键是否按下。第一个参数是GLFW窗口句柄，第二个参数是一个GLFW常量，代表一个键。
	//GLFW_KEY_ESCAPE表示Esc键。如果Esc键按下了，glfwGetKey将返回GLFW_PRESS（值为1），否则返回GLFW_RELEASE（值为0）。
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//WASD移动摄像机
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

// 当用户改变窗口的大小的时候，视口也应该被调整。
//对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//OpenGL渲染窗口的尺寸大小
	//glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
	glViewport(0, 0, width, height);
}

//响应鼠标事件 创建鼠标的回调函数
//xpos,ypos鼠标的位置
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//如果为第一次进入 记录当前鼠标位置 为初始化鼠标位置
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//计算当前帧和上一帧的位置差
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 这里如果不反过来 实际操作过程中向上移动摄像机 物体也会跟着向上移动 实际应该向下

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

//响应鼠标滚轮
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}