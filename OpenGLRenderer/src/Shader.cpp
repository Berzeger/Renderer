#include "Shader.h"
#include "Macros.h"

#include <iostream>
#include <sstream>
#include <fstream>

unsigned int Shader::CompileShader(unsigned int type, const std::string & source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GlCall(glShaderSource(id, 1, &src, nullptr));
	GlCall(glCompileShader(id));

	int result;
	GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char * message = (char *)alloca(length * sizeof(char));
		GlCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GlCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

std::string Shader::ParseShader(const std::string & filepath, GLenum type) const
{
	std::ifstream stream(filepath);

	if (!stream.is_open())
	{
		std::cout << "Failed to open " << filepath << "!" << std::endl;
		return "";
	}
	
	std::string line;
	std::stringstream ss;

	while (getline(stream, line))
	{
		ss << line << '\n';
	}

	return ss.str();
}

unsigned int Shader::CreateShader(const std::string & vertexShader, const std::string & fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));
	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));

	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));

	return program;
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
	std::string vertexSource = ParseShader(vertexPath, GL_VERTEX_SHADER);
	std::string fragmentSource = ParseShader(fragmentPath, GL_FRAGMENT_SHADER);
	ProgramId = CreateShader(vertexSource, fragmentSource);
}

void Shader::use()
{
	GlCall(glUseProgram(ProgramId));
}

void Shader::setBool(const std::string & name, bool value) const
{
	GlCall(glUniform1i(glGetUniformLocation(ProgramId, name.c_str()), (int)value));
}

void Shader::setInt(const std::string & name, int value) const
{
	GlCall(glUniform1i(glGetUniformLocation(ProgramId, name.c_str()), value));
}

void Shader::setFloat(const std::string & name, float value) const
{
	GlCall(glUniform1f(glGetUniformLocation(ProgramId, name.c_str()), value));
}

void Shader::setMat4(const std::string & name, const glm::mat4& value) const
{
	GlCall(glUniformMatrix4fv(glGetUniformLocation(ProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)));
}
