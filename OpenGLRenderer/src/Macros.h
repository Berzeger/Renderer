#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
	x;\
	ASSERT(GlLogCall(#x, __FILE__, __LINE__))

static void GlClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GlLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ": " << gluErrorString(error) << ") in " << function <<
			" " << file << ": " << line << std::endl;
		return false;
	}

	return true;
}