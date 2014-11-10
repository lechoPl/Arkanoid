#include <cstdlib>
#include <cstdio>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "shader.hpp"
#include "game.h"
#include "model.h"

using namespace std;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
	GLFWwindow* window;
	CGame game;

	if (!glfwInit())
		return -1;

	//set error callback
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Arkanoid", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//set key callback
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		return -1;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint programID = LoadShaders("VertexShader.vshader", "FragmentShader.fshader");

	GLuint ColorID = glGetUniformLocation(programID, "colorData");
	GLuint PositionID = glGetUniformLocation(programID, "position");
	GLuint ScaleID = glGetUniformLocation(programID, "scale");

	SObjectsDrawDesc objectsData;
	vector<GLfloat> vertex_data = getVertexBufferData(game, &objectsData);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_data.size()*sizeof(GLfloat), &vertex_data[0], GL_STATIC_DRAW);

	double currentTime = glfwGetTime();
	double lastTime = currentTime;
	double deltaTime;
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		game.update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programID);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glUniform3f(ColorID, 0.3f, 0.3f, 0.3f);
		glUniform2f(PositionID, 0.f, 0.f);
		glUniform1f(ScaleID, 1.f);
		objectsData.boardData.draw();

		glUniform3f(ColorID, 1.f, 0.f, 0.f);
		glUniform2f(PositionID, game.getBall()->getPos().x, game.getBall()->getPos().y);
		objectsData.ballData.draw();

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();
	return 0;
}