#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_Id;

	unsigned int CompileShader(unsigned int type, const std::string& source);
	std::string ParseShader(const std::string& filepath, GLenum type) const;
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

public:
	unsigned int ProgramId;

	Shader(const std::string &vertexPath, const std::string &fragmentPath);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string &name, const glm::mat4& value) const;
};

