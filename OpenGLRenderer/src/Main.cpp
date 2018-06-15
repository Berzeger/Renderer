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
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

extern "C" {
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX;
float lastY;
bool firstMouse = true;

static void framebuffer_resize_callback(GLFWwindow* window, int fbH, int fbW)
{
	glViewport(0, 0, fbH, fbW);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)(xpos - lastX);
	float yoffset = (float)(lastY - ypos);
	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.processMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScrollWheel((float)yoffset);
}

void processInput(GLFWwindow* window, float dt)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.processKeyboard(FORWARD, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyboard(BACKWARDS, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyboard(LEFT, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyboard(RIGHT, dt);
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
	lastX = WINDOW_WIDTH * 0.5f;
	lastY = WINDOW_HEIGHT * 0.5f;
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
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glViewport(0, 0, framebufferHeight, framebufferHeight);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error while initializing GLEW!" << std::endl;
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	GlCall(glEnable(GL_DEPTH_TEST));

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	GLuint cubeVao;
	GlCall(glGenVertexArrays(1, &cubeVao));
	GlCall(glBindVertexArray(cubeVao));

	// vertex info (vbo)
	VertexBuffer vertexBuffer(vertices, sizeof(vertices));

	// positions
	GlCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GlCall(glEnableVertexAttribArray(0));

	// normals
	GlCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3)));
	GlCall(glEnableVertexAttribArray(1));

	// texture coords
	GlCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6)));
	GlCall(glEnableVertexAttribArray(2));

	// lamp
	GLuint lightVao;
	GlCall(glGenVertexArrays(1, &lightVao));
	GlCall(glBindVertexArray(lightVao));

	vertexBuffer.bind();

	GlCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GlCall(glEnableVertexAttribArray(0));

	Shader cubeShader("res/shaders/lighting_map.vs", "res/shaders/lighting_map.fs");
	Shader lampShader("res/shaders/lamp.vs", "res/shaders/lamp.fs");

	Texture2D cubeTexture("res/images/container2_specular.png", false, 0);
	Texture2D cubeSpecularTexture("res/images/container2_specular.png", false, 1);
	Texture2D cubeEmissiveTexture("res/images/matrix_borders.jpg", false, 2);

	glm::vec4 lightPosition(1.2f, 1.0f, 2.0f, 1.0f);
	glm::vec4 lightDirection(-0.2f, -1.0f, -0.3f, 0.0f);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float second = 0.0f;
	unsigned int frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		// -----
		processInput(window, deltaTime);

		// Render
		// ------
		GlCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		cubeShader.use();

		cubeShader.setVec4("camPos", glm::vec4(camera.getPosition(), 1.0f));
		cubeShader.setFloat("time", glfwGetTime());

		cubeTexture.setActive();
		cubeShader.setInt("material.diffuse", 0);
		cubeSpecularTexture.setActive();
		cubeShader.setInt("material.specular", 1);
		cubeEmissiveTexture.setActive();
		cubeShader.setInt("material.emissive", 2);
		cubeShader.setFloat("material.shininess", 32.0f);

		cubeShader.setVec4("light.vector", lightDirection);
		cubeShader.setVec4("light.ambient", 0.2f, 0.2f, 0.2f, 0.0f);
		cubeShader.setVec4("light.diffuse", 0.5f, 0.5f, 0.5f, 0.0f); // darken the light a bit to fit the scene
		cubeShader.setVec4("light.specular", 1.0f, 1.0f, 1.0f, 0.0f);

		//cubeShader.setVec3("material.ambient", 0.329412f, 0.223529f, 0.027451f);
		//cubeShader.setVec3("material.diffuse", 0.780392f, 0.568627f, 0.113725f);
		//cubeShader.setVec3("material.specular", 0.992157f, 0.941176f, 0.807843f);
		//cubeShader.setFloat("material.shininess", 27.8974f);

		//cubeShader.setVec3("light.position", lightPosition);
		//cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		//cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
		//cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		
		// View/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), ((float)WINDOW_WIDTH) / WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);
		
		// World transformation
		glm::mat4 model(1.0f);
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("transInvModel", glm::transpose(glm::inverse(model)));

		// Render the cube
		GlCall(glBindVertexArray(cubeVao));
		GlCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		// Now the lamp object
		lampShader.use();
		lampShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(lightPosition));
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);

		GlCall(glBindVertexArray(lightVao));
		GlCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		// swaps front and back buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// FPS
		// ---
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

	GlCall(glDeleteVertexArrays(1, &cubeVao));
	GlCall(glDeleteVertexArrays(1, &lightVao));

	glfwTerminate();

	return 0;
}