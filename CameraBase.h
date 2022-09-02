#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// ����ö�� ��������
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Ĭ�ϵ����������
const float YAW = -90.0f;
const float PITCH = 0.0f;
//const float SPEED = 2.5f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// �������
class Camera
{
public:
	// �����������
	glm::vec3 Position;     //λ��
	glm::vec3 Front;        //��ǰ������
	glm::vec3 Up;           //���� 
	glm::vec3 Right;        //����
	glm::vec3 WorldUp;      //��������ϵ�µ��������� 
	// ŷ���� ƫ�����븩����
	float Yaw;
	float Pitch;
	// ������� �ƶ��ٶ� ��������� �ӽ�
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//���캯�� ��ʼ���������� ���� ��������ʼ��
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// ��ʼ�� �ñ�����ʼ��
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// ������ľ���
	glm::mat4 GetViewMatrix()
	{

		//��һ�������� �������λ��
		//�ڶ����������Ŀ�� ��������������ķ����  Position - (Target)  Target =  Position + Front
		//������λ���޹� �˴���Ϊ���������������ƶ� ���������ע����Ŀ��ǰ�� 
		//�������� ��������ϵ�����ϵ�����
		return glm::lookAt(Position, Position + Front, Up);
	}

	// ����������������ƶ�
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

	// ��Ӧ����ƶ� ����Pitch��Yaw��ֵ ����������������ķ�������
	//ˮƽ�ƶ�Ӱ��ƫ����Yaw ��ֱ�ƶ�Ӱ�츩���� Pitch
	//������һ֡����λ�ã��ڵ�ǰ֡�����ǵ�ǰ�������λ������һ֡��λ�������١�
	//���ˮƽ/��ֱ���Խ����ô�����ǻ�ƫ���Ǿ͸ı�Խ��Ҳ�����������Ҫ�ƶ�����ľ���
	void ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch = true)
	{
		//��ֹ�����ƶ� ����ƫ����/���������
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		//����Pitch��Yaw��ֵ
		Yaw += float(xoffset);
		Pitch += float(yoffset);

		// ȷ����ȷ����ʾЧ�� ��Ϊ�����Ƕ���������
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// ����������ķ�������
		updateCameraVectors();
	}

	// ���������
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
	// ͨ�������Ǻ�ƫ�����������Եõ������ķ�������
	void updateCameraVectors()
	{
		// ��ʼ��
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// ��������������ҵ����� ����һ��
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif

