#include "Shader.h"
#include "Macros.h"

#include <iostream>
#include <sstream>
#include <fstream>

unsigned int Shader::compileShader(unsigned int type, const std::string & source)
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

std::string Shader::parseShader(const std::string & filepath, GLenum type) const
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

unsigned int Shader::createShader(const std::string & vertexShader, const std::string & fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

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
	std::string vertexSource = parseShader(vertexPath, GL_VERTEX_SHADER);
	std::string fragmentSource = parseShader(fragmentPath, GL_FRAGMENT_SHADER);
	ProgramId = createShader(vertexSource, fragmentSource);
	m_UniformLocations = new std::unordered_map<std::string, GLint>();
	cacheUniformLocations();
}

Shader::~Shader()
{
	glDeleteProgram(ProgramId);
	delete m_UniformLocations;
}

void Shader::use()
{
	GlCall(glUseProgram(ProgramId));
}

void Shader::cacheUniformLocations() const
{
	GLint maxUniformNameLen, nUniforms;
	GlCall(glGetProgramiv(ProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLen));
	GlCall(glGetProgramiv(ProgramId, GL_ACTIVE_UNIFORMS, &nUniforms));

	GLint read, size;
	GLenum type;
	const GLsizei bufSize = 32; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL

	for (GLint i = 0; i < nUniforms; ++i)
	{
		glGetActiveUniform(ProgramId, i, maxUniformNameLen, &read, &size, &type, name);
		(*m_UniformLocations)[name] = glGetUniformLocation(ProgramId, name);
	}
}

void Shader::setBool(const std::string & name, bool value) const
{
	GlCall(glUniform1i((*m_UniformLocations)[name], (int)value));
}

void Shader::setInt(const std::string & name, int value) const
{
	GlCall(glUniform1i((*m_UniformLocations)[name], value));
}

void Shader::setFloat(const std::string & name, float value) const
{
	GlCall(glUniform1f((*m_UniformLocations)[name], value));
}

void Shader::setMat4(const std::string & name, const glm::mat4& value) const
{
	GlCall(glUniformMatrix4fv((*m_UniformLocations)[name], 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::setVec3(const std::string &name, const glm::vec3& value) const
{
	GlCall(glUniform3fv((*m_UniformLocations)[name], 1, &value[0]));
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	GlCall(glUniform3f((*m_UniformLocations)[name], x, y, z));
}
