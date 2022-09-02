#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>   //����glad����ȡ���ñ����OpenGL�ļ�
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//����ID
	unsigned int ID;
	// ���캯�� ������ɫ��
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// ���ļ�·���л�ȡ����/ƬԴ��ɫ��
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ȷ��ifstream�ļ������׳��쳣
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// ���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// ��ȡ�ļ������ݵ����ݻ�������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// ��ȡ�ɹ����ر��ļ�
			vShaderFile.close();
			fShaderFile.close();
			// ת������������Ϊ�ַ���
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//������ɫ��
		unsigned int vertex, fragment;
		// ������ɫ��
		//����������ɫ��
		vertex = glCreateShader(GL_VERTEX_SHADER);
		//��ɫ��Դ�븽�ӵ������ϣ�Ȼ�����
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		//����Ƿ����ɹ�
		checkCompileErrors(vertex, "VERTEX");
		// ƬԴ��ɫ��
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		//��ɫ��Դ�븽�ӵ������ϣ�Ȼ�����
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		//����Ƿ����ɹ�
		checkCompileErrors(fragment, "FRAGMENT");
		//����Shader�����Ӷ�����ƬԴ
		//�������Ӷ���
		ID = glCreateProgram();
		//��ɫ�����ӵ��˳����ϣ�Ȼ����glLinkProgram����
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		//����Ƿ����ӳ���
		checkCompileErrors(ID, "PROGRAM");
		//����ɫ���������ӵ���������Ժ�ɾ����ɫ�����󣬲�����Ҫ����
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	//�������ӳ��򣬼�����ɫ������ʼ��Ⱦ
	void use()
	{
		glUseProgram(ID);
	}

	//����Uniform������ֵ
	//��ѯuniform��ַ��Ҫ����֮ǰʹ�ù���ɫ������
		//���Ǹ���һ��uniform֮ǰ�������ʹ�ó��򣨵���glUseProgram)����Ϊ�����ڵ�ǰ�������ɫ������������uniform�ġ�
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat4(const std::string& name, glm::mat4 mat)
	{
		int matLoc = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(mat));
	}


private:
	// ����Ƿ���� �ж��Ƕ���/ƬԴ �������ӳ���
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif