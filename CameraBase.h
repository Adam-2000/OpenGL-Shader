#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// 定义枚举 上下左右
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// 默认的摄像机参数
const float YAW = -90.0f;
const float PITCH = 0.0f;
//const float SPEED = 2.5f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// 摄像机类
class Camera
{
public:
	// 摄像机的属性
	glm::vec3 Position;     //位置
	glm::vec3 Front;        //向前的向量
	glm::vec3 Up;           //向上 
	glm::vec3 Right;        //向右
	glm::vec3 WorldUp;      //世界坐标系下的向上向量 
	// 欧拉角 偏航角与俯仰角
	float Yaw;
	float Pitch;
	// 摄像机的 移动速度 鼠标灵敏度 视角
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//构造函数 初始化摄像机类的 变量 用向量初始化
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// 初始化 用标量初始化
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// 摄像机的矩阵
	glm::mat4 GetViewMatrix()
	{

		//第一个参数是 摄像机的位置
		//第二个是摄像机目标 用来计算摄像机的方向的  Position - (Target)  Target =  Position + Front
		//方向与位置无关 此处是为了无论摄像机如何移动 摄像机都会注视着目标前方 
		//第三个是 世界坐标系下向上的向量
		return glm::lookAt(Position, Position + Front, Up);
	}

	// 摄像机的上下左右移动
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
		if (direction == UP)
			Position += Up * velocity;
		if (direction == DOWN)
			Position -= Up * velocity;
	}

	// 响应鼠标移动 更改Pitch与Yaw的值 以用来计算摄像机的方向向量
	//水平移动影响偏航角Yaw 竖直移动影响俯仰角 Pitch
	//储存上一帧鼠标的位置，在当前帧中我们当前计算鼠标位置与上一帧的位置相差多少。
	//如果水平/竖直差别越大那么俯仰角或偏航角就改变越大，也就是摄像机需要移动更多的距离
	void ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch = true)
	{
		//防止过速移动 乘以偏移量/鼠标灵敏度
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		//更改Pitch与Yaw的值
		Yaw += float(xoffset);
		Pitch += float(yoffset);

		// 确保正确的显示效果 ，为俯仰角度添加限制
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// 更新摄像机的方向向量
		updateCameraVectors();
	}

	// 缩放摄像机
	void ProcessMouseScroll(double yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 90.0f)
			Zoom -= float(yoffset);
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 90.0f)
			Zoom = 90.0f;
	}

private:
	// 通过俯仰角和偏航角来计算以得到真正的方向向量
	void updateCameraVectors()
	{
		// 初始化
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// 计算出向上与向右的向量 并归一化
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif


