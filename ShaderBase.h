#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>   //包含glad来获取所用必须的OpenGL文件
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//程序ID
	unsigned int ID;
	// 构造函数 构建着色器
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 从文件路径中获取顶点/片源着色器
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// 确保ifstream文件可以抛出异常
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// 打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// 读取文件的内容到数据缓冲流中
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// 读取成功，关闭文件
			vShaderFile.close();
			fShaderFile.close();
			// 转换数据流内容为字符串
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//编译着色器
		unsigned int vertex, fragment;
		// 顶点着色器
		//创建顶点着色器
		vertex = glCreateShader(GL_VERTEX_SHADER);
		//着色器源码附加到对象上，然后编译
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		//检查是否编译成功
		checkCompileErrors(vertex, "VERTEX");
		// 片源着色器
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		//着色器源码附加到对象上，然后编译
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		//检查是否编译成功
		checkCompileErrors(fragment, "FRAGMENT");
		//链接Shader，链接顶点与片源
		//创建链接对象
		ID = glCreateProgram();
		//着色器附加到了程序上，然后用glLinkProgram链接
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		//检查是否链接出错
		checkCompileErrors(ID, "PROGRAM");
		//把着色器对象链接到程序对象以后，删除着色器对象，不再需要它们
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	//激活链接程序，激活着色器，开始渲染
	void use()
	{
		glUseProgram(ID);
	}

	//设置Uniform变量的值
	//查询uniform地址不要求你之前使用过着色器程序
		//但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
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
	// 检差是否出错 判断是顶点/片源 还是链接程序
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