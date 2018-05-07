#define GLEW_STATIC

#include <Windows.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "stb_image.h"
#include "Macros.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Timer.h"

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

float mixValue = 0.2f;

static void framebuffer_resize_callback(GLFWwindow* window, int fbH, int fbW)
{
	glViewport(0, 0, fbH, fbW);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		mixValue += 0.1f;
	} 
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		mixValue -= 0.1f;
	}

	if (mixValue > 1.0f)
	{
		mixValue = 1.0f;
	} 
	else if (mixValue < 0.0f)
	{
		mixValue = 0.0f;
	}
}

int main(int argc, char ** argv)
{
	// Initialize the library
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glewExperimental = GL_TRUE;

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Title", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glViewport(0, 0, framebufferHeight, framebufferHeight);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint vao = 0;
	GlCall(glGenVertexArrays(1, &vao));
	GlCall(glBindVertexArray(vao));

	// vertex info
	unsigned int buffer;
	GlCall(glGenBuffers(1, &buffer));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GlCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	// vertex indices
	unsigned int ibo;
	GlCall(glGenBuffers(1, &ibo));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	// positions
	GlCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));
	GlCall(glEnableVertexAttribArray(0));

	// colors
	GlCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GlCall(glEnableVertexAttribArray(1));

	// texture coords
	GlCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	GlCall(glEnableVertexAttribArray(2));

	// texture
	Texture2D texture1("res/images/wall.jpg", false, GL_TEXTURE0);
	Texture2D texture2("res/images/awesomeface.png", true, GL_TEXTURE1);

	Shader shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);
	shader.setFloat("ratio", 1.0f);

	Shader shader2("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
	shader2.use();
	shader2.setInt("texture1", 0);
	shader2.setInt("texture2", 1);
	shader2.setFloat("ratio", 1.0f);

	Timer timer;
	float deltaTime = 0.0f;
	float second = 0.0f;
	unsigned int frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		timer.start();
		GlCall(glClear(GL_COLOR_BUFFER_BIT));

		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setMat4("transform", trans);
		shader.use();

		GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		float sinus = glm::sin(glfwGetTime());
		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans = glm::scale(trans, glm::vec3(sinus, sinus, 1.0f));
		shader2.setMat4("transform", trans);
		shader2.use();

		GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		// swaps front and back buffers
		glfwSwapBuffers(window);
		shader.setFloat("ratio", mixValue);

		glfwPollEvents();
		timer.end();
		deltaTime = timer.getDeltaTime();
		second += deltaTime;
		frames++;

		if (second >= 1.0f)
		{
			float fps = frames / second;
			std::cout << fps << " FPS (" << 1000 / fps << " ms per frame)" << "\n";
			second -= 1.0f;
			frames = 0;
		}
	}

	GlCall(glDeleteProgram(shader.ProgramId));

	glfwTerminate();

	return 0;
}